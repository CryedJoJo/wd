/**
 * Project Untitled
 */

#include "threadPool.h"
#include "Task.h"

/**
 * threadPool implementation
 */

/**
 * @param threadCount
 */
threadPool::threadPool(size_t threadCount, size_t queSize)
    : threadCount_(threadCount)
    , threads_()
    , taskQue_(queSize)
    , isExit_(true)
{
}

/**
 * @return void
 */
void threadPool::init()
{
	for(size_t i = 0; i < threadCount_; ++i) {
		threads_.emplace_back(thread(&threadPool::doTask, this));
	}
	isExit_ = false;
}

/**
 * @return bool
 */
bool threadPool::isExit()
{
	return isExit_;
}

/**
 * @return bool
 */
bool threadPool::addTask(Task *task)
{
	taskQue_.pushElem(task);
	return true;
}

/**
 * @return void
 */
void threadPool::start()
{
	try {
		init();
	} catch(...) {
		// throw
	}
}

/**
 * @return void
 */
void threadPool::stop()
{
	while(!taskQue_.isEmpty()) {
		std::cout << "the tasks haven't been finish yet!" << std::endl;
	}
	isExit_ = true;
	taskQue_.exit();
	taskQue_.wakeupAll();
	for(auto &t : threads_) {
		t.join();
	}
}

/**
 * @return Task*
 */
Task *threadPool::getTask()
{
	return taskQue_.popElem();
}

/**
 * @return void
 */
void threadPool::doTask()
{
	while(1) {
		Task* tk = getTask();
		if(tk){
			tk->process();
		} else {
			std::cout << "tk is nullptr" << std::endl;
		}
		if(isExit_) {
			break;
		}
	}
}