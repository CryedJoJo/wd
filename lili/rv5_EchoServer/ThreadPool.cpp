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
	//1. ThreadPool 析构死循环
	//修改前的代码：
	/*
	while(!isExit_ || !taskQue_.isEmpty()) {
		::sleep(1);
	}
	taskQue_.wakeUpAll();
	for(auto &th : threads_) {
		th.join();
	}
	*/
	//问题：exit() 从未被调用时 isExit_ 永远为 false，析构死循环无法退出；
	//     且 worker 被唤醒后不再消费队列，等待 queue 空也是等不到的
	//修改过后的代码：置位退出标志并唤醒所有阻塞在 popTask 的线程，join 等待线程结束（未消费的任务被丢弃）
	exit();
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
	//1. 只置位 isExit_ 不唤醒任务队列，worker 阻塞在 popTask 的 isEmpty_.wait() 上永远退出不了
	//修改前的代码：
	// isExit_ = true;
	//修改过后的代码：
	isExit_ = true;
	taskQue_.wakeUpAll();
}

// void ThreadPool::runInLoop()
// {
// 	eventLoopPtr_->write();
// }