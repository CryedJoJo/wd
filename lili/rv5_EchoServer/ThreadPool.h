/**
 * Project Untitled
 */

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "TaskQueue.h"

#include <functional>
#include <vector>
#include <thread>
#include <atomic>

using std::function;
using std::vector;
using std::thread;
using Task = std::function<void()>;
using std::atomic;

class EventLoop;

class ThreadPool {
public:
	// ThreadPool(size_t threadCapacity_, size_t Qsize, EventLoop *eventLoopPtr);
	ThreadPool(size_t threadCapacity_, size_t Qsize);
	~ThreadPool();
	bool addTask(Task &&);
	void exit();

private:
	void init();
	Task getTask();

	/**
 * @param Task&&
 */

	void doTask();

	// void runInLoop(); //不要thread pool 负责

	size_t         Qsize_;
	TaskQueue      taskQue_;
	vector<thread> threads_;
	atomic<bool>           isExit_;
	size_t         threadCapacity_;
	// EventLoop *eventLoopPtr_; //由TcpConnection持有
};

#endif //_THREADPOOL_H