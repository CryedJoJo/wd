/**
 * Project Untitled
 */

#include "threadPool.h"
#include <unistd.h>

/**
 * threadPool implementation
 */

/**
 * @param size
 */
threadPool::threadPool(size_t threadNum, size_t Qsize)
    : threadNum_(threadNum)
    , threadContainer_()
    , taskQue_(Qsize)
    , isExit_(true)
    , taskAddFinish_(true)
{
	start();
}

/**
 * @return void
 */
void threadPool::start()
{
	try {
		init();
	} catch(...) {
	}
}

/**
 * @return bool
 */
bool threadPool::init()
{

	isExit_        = false;
	taskAddFinish_ = false;
	for(size_t i = 0; i < threadNum_; ++i) {
		threadContainer_.emplace_back(thread(&threadPool::doTask, this));
	}
	return true;
}

/**
 * @return bool
 */
bool threadPool::getStatus()
{
	return false;
}

/**
 * @param task
 * @return bool
 */
bool threadPool::addTask(Task &&task)
{
	return taskQue_.pushElem(std::move(task)); //合理。task 是具名右值引用参数（左值），不 move 会拷贝
}

void threadPool::taskAddFinish()
{
	taskAddFinish_ = true;
}

/**
 * @return void
 */
void threadPool::exit()
{
	//任务投放结束 可调用exit
	isExit_ = true;
}

/**
 * @return void
 */
void threadPool::stop()
{
	while(taskQue_.curSize() != 0 || !taskAddFinish_) {
		std::cout << "threadpool is working..." << std::endl;
		// ::sleep(1);
	}
	taskQue_.wakeupAll();
	exit();
	for(auto &t : threadContainer_) {
		t.join();
	}
}

/**
 * @return Task
 */
Task threadPool::getTask()
{
	return std::move(taskQue_.popElem()); //不合理。popElem返回的操作数本身就是纯右值，阻止拷贝省略，多一次 move。改为 return taskQue_.popElem();
}

/**
 * @return void
 */
void threadPool::doTask()
{

	while(1) {
		if(isExit_) {
			return;
		}
		Task taskcb = std::move(getTask()); //不合理。getTask返回的本身就是右值，move之后也是右值，没有意义
		if(taskcb) {
			taskcb();
		}
	}
}