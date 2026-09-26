/**
 * Project Untitled
 */

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "taskQueue.h"

#include <iostream>
#include <functional>
#include <thread>
#include <vector>

using Task = std::function<void()>;
using std::vector;
using std::thread;

class threadPool {
public:
	/**
 * @param size
 */
	threadPool(size_t threadNum, size_t Qsize);
	~threadPool()
	{
		taskAddFinish();
		stop();
	}

	bool init();

	bool getStatus();

	/**
 * @param task
 */
	bool addTask(Task &&task);

	void start();
	void taskAddFinish();
	void stop();

private:
	size_t         threadNum_;
	vector<thread> threadContainer_;
	taskQueue      taskQue_;
	bool           isExit_;
	bool           taskAddFinish_;

	Task getTask();
	void doTask();
	void exit();
};

#endif //_THREADPOOL_H