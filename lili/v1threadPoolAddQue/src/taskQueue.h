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

class taskQueue {
public: 
    taskQueue(int size);
	~taskQueue();

	bool isFull();

	bool isEmpty();

	int curSize();

	void pushElem(int elem);

	int popElem();
private:
	int                queSize_;
	queue<int>         que_;
	mutex              mutex_;
	condition_variable notFull_;
	condition_variable notEmpty_;
	
};

#endif //_TASKQUEUE_H