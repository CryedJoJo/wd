/**
 * Project Untitled
 */

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "TaskQueue.h"

#include <functional>
#include <vector>
#include <thread>

using std::function;
using std::vector;
using std::thread;
using Task = std::function<void()>;

class ThreadPool {
public:
	ThreadPool(size_t threadCapacity_, size_t Qsize);

	~ThreadPool();
	void addTask(Task &&);
	void exit();

private:
	Task getTask();

	/**
 * @param Task&&
 */

	void doTask();
	void init();

	size_t         Qsize_;
	TaskQueue      taskQue_;
	vector<thread> threads_;
	bool           isExit_;
	size_t         threadCapacity_;
};

#endif //_THREADPOOL_H