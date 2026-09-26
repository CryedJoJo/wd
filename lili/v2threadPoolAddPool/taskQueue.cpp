/**
 * Project Untitled
 */

#include "taskQueue.h"
#include "Task.h"

/**
 * taskQueue implementation
 */

/**
 * @param size
 */
taskQueue::taskQueue(size_t size)
    : queSize_(size)
    , que_()
    , mutex_()
    , notFull_()
    , notEmpty_()
    , exit_(false)
{
}

/**
 * @return bool
 */
bool taskQueue::isFull()
{
	return queSize_ == curSize();
}

/**
 * @return bool
 */
bool taskQueue::isEmpty()
{
	return 0 == curSize();
}

/**
 * @return int
 */
size_t taskQueue::curSize()
{
	/*代码中taskQueue中的que_是共享数据，进行了加锁存取，但是执行curSize时，函数里面只是跑了一下que_.size()，这种读操作一般需要加锁吗，正常的商业代码中对这种场景是怎么处理的？也加锁？

	Thought : 2.1s 简短结论：需要加锁（或者用原子变量）， "只是读一下 size" 并不意味着线程安全。
	              为什么读也要加锁
	              std::queue::size() 不是原子的，它去读 deque     内部的成员（begin
	          / end 迭代器、size 计数等）。如果此刻另一个线程正在 push() / pop()，那就是典型的 data race → C++ 里属于 UB，
			  可能读到撕裂 / 过期的值，甚至崩溃，而不只是 "旧值"这么温和。 而且这段代码里问题更明显：
	    - curSize() 是 public 的，任何线程都能不经锁调用；
	    - pushElem / popElem 里调用 isFull() / isEmpty() 时 *已经持有 mutex_ *，所以那几处是安全的；
	    - 但 exit_ 是普通 bool，exit() 无锁写、worker 持锁读，同样是 race；wakeupAll() 也没持锁。
	        商业代码通常怎么处理
	            三种主流做法，按推荐度： 1. 统一加锁（最常见、最稳）
	                size_t
	                taskQueue::curSize()
	{
		lock_guard<mutex> lg(mutex_);
		return que_.size();
	}
	代价很低，语义清晰，所有对队列状态的访问都走同一把锁，不易出错。
	2. 原子计数器
	std::atomic<size_t>
	size_{0}; // push/pop 时 ++/--；curSize 返回 load()
	适合高频查询 size 的场景，避免读也要抢锁。但队列本身仍要锁保护，size_ 要和 queue 操作保持一致，维护成本略高。 3. 收紧可见性
	如果 curSize 只在类内部持锁时使用，就把它设为 private，从接口上杜绝 "无锁调用"的可能。这才是最根本的——问题不在读本身，而在暴露了一个不保证同步的 public 接口。
	一个更好的重构方向
	现在    isFull
	        / isEmpty / curSize 是 public 且各自不加锁，依赖调用者 "自觉持锁"，非常容易误用。更商业化的写法是把判断逻辑内联进 		pushElem / popElem（它们本来就持锁），或者所有 public 方法内部都自己 lock_guard，让锁的边界和 public 边界对齐。 另			外注意：isFull() 里的 queSize_
	    == curSize() 在持锁环境下没问题，但如果 curSize() 改成内部加锁，就会和 pushElem 里的锁重入死锁（std::mutex 不可重入）。所以正确做法通常是加一个私有的 curSizeUnlocked()，public 版加锁、内部版不加锁——这也是很多商业库的惯例。 
		*/
		return que_.size();
}

/**
 * @return void
 */
void taskQueue::pushElem(Task *task)
{
	unique_lock<mutex> ul(mutex_);

	// notFull_.wait(ul, [this]() { return !isFull(); });
	while(isFull()) {
		// std::cout << __FILE__ << " " << __LINE__ << std::endl;
		if(exit_) {
			std::cout << "task queue exit line:" << __LINE__ << std::endl;
			return;
		}
		notFull_.wait(ul);
	}
	que_.push(task);
	notEmpty_.notify_one();
}

/**
 * @return Task* 
 */
Task *taskQueue::popElem()
{
	unique_lock<mutex> ul(mutex_);
	// notEmpty_.wait(ul, [this]() { return !isEmpty(); });
	while(isEmpty()) {
		if(exit_) {
			std::cout << "task queue exit line:" <<  __LINE__ << std::endl;
			return nullptr;
		}
		//std::cout << __FILE__ << " " << __LINE__ << std::endl; //为什么我加了这一行代码 程序就不会卡住，注释了之后，就会卡住不能结束？ "海森堡bug"
		notEmpty_.wait(ul);
	}
	Task *tk = que_.front();
	que_.pop();
	return tk;
}

void taskQueue::wakeupAll()
{
	notFull_.notify_all();
	notEmpty_.notify_all();
}
