/**
 * Project Untitled
 */

#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>

using std::mutex;
using std::condition_variable;
using std::queue;
using std::unique_lock;

class Task;

class taskQueue {
public:
	/**
 * @param size
 */
	taskQueue(size_t size);

	bool isFull();

	bool isEmpty();

	size_t curSize();

	void pushElem(Task *task);

	void wakeupAll();

	void exit()
	{
		exit_ = true;
	}

	/**
 * @param task
 */
	Task *popElem();

private:
	size_t             queSize_;
	mutex              mutex_;
	condition_variable notFull_;
	condition_variable notEmpty_;
	queue<Task *>      que_;
	bool               exit_;
};

#endif //_TASKQUEUE_H