/**
 * Project Untitled
 */

#include "taskQueue.h"

/**
 * taskQueue implementation
 */

/**
 * @param size
 */
taskQueue::taskQueue(size_t size)
    : queSize_(size)
    , mutex_()
    , notFull_()
    , notEmpty_()
    , que_()
    , exitFlag_(false)
{
}

/**
 * @return bool
 */
bool taskQueue::isFull()
{
	return queSize_ == que_.size();
}

/**
 * @return bool
 */
bool taskQueue::isEmpty()
{
	return 0 == que_.size();
}

/**
 * @return size_t
 */
size_t taskQueue::curSize()
{
	unique_lock<mutex> ul(mutex_);
	return que_.size();
}

/**
 * @param task
 * @return void
 */
bool taskQueue::pushElem(ElemType &&task)
{
	unique_lock<mutex> ul(mutex_);
	while(isFull()) {
		// if(exitFlag_){ //没有可能要退出之前 还卡这儿，根本不可能。因为如果还isFull 那么exitFlag_ 不可能被设置成true，因为必须是isEmpty了，exitFlag_ 才会是true
		// 	return;
		// }
		notFull_.wait(ul);
	}
	que_.push(std::move(task)); // 合理。task 是具名右值引用参数，move 进队列避免拷贝
	notEmpty_.notify_one();
	return true;
}

/**
 * @return ElemType
 */
ElemType taskQueue::popElem()
{
	unique_lock<mutex> ul(mutex_);
	while(isEmpty()) {
		if(exitFlag_) {
			return nullptr;
		}
		notEmpty_.wait(ul);
	}
	ElemType taskcb = std::move(que_.front()); //合理，front() 返回的是T&,左值 使用move可以走移动赋值运算符函数，减少一次拷贝
	que_.pop();
	notFull_.notify_one();
	return taskcb;
}

/**
 * @return void
 */
void taskQueue::wakeupAll()
{
	exitQ();
	notEmpty_.notify_all();
}

/**
 * @return void
 */
void taskQueue::exitQ()
{
	unique_lock<mutex> ul(mutex_);
	exitFlag_ = true;
}