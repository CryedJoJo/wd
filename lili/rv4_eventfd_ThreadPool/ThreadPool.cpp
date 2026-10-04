/**
 * Project Untitled
 */

#include "ThreadPool.h"
#include "EventLoop.h"
#include <unistd.h>
#include <iostream>

/**
 * ThreadPool implementation
 */

// ThreadPool::ThreadPool(size_t threadCapacity, size_t Qsize, EventLoop *eventLoopPtr)
//     : Qsize_(Qsize)
//     , taskQue_(Qsize_)
//     , threads_()
//     , isExit_(false)
//     , threadCapacity_(threadCapacity)
// {
// 	eventLoopPtr_ = eventLoopPtr;
// 	init();
// }

ThreadPool::ThreadPool(size_t threadCapacity, size_t Qsize)
    : Qsize_(Qsize)
    , taskQue_(Qsize_)
    , threads_()
    , isExit_(false)
    , threadCapacity_(threadCapacity)
{
	init();
}

ThreadPool::~ThreadPool()
{
	while(!isExit_ || !taskQue_.isEmpty()) {
		::sleep(1);
	}
	taskQue_.wakeUpAll();
	for(auto &th : threads_) {
		th.join();
	}
}

/**
 * @return void
 */
void ThreadPool::init()
{
	for(size_t i = 0; i < threadCapacity_; ++i) {
		threads_.emplace_back(thread(&ThreadPool::doTask, this));
		std::cout << "add " << i + 1 << "'s thread in vector" << std::endl;
	}
}

/**
 * @return Task
 */
Task ThreadPool::getTask()
{
	return taskQue_.popTask();
}

/**
 * @param Task&&
 * @return void
 */
bool ThreadPool::addTask(Task &&task)
{
	return taskQue_.pushTask(std::move(task));
}

void ThreadPool::doTask()
{
	while(!isExit_) {
		Task tk = getTask();
		if(tk) {
			tk();
			// runInLoop();
		}
	}
}

/**
 * @return void
 */
void ThreadPool::exit()
{
	isExit_ = true;
}

// void ThreadPool::runInLoop()
// {
// 	eventLoopPtr_->write();
// }