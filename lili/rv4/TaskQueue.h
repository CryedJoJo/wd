/**
 * Project Untitled
 */

#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include <functional>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

using std::function;
using std::queue;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using Task = std::function<void()>;

class TaskQueue {
public:
	TaskQueue(size_t capacity);

	~TaskQueue();

	bool isEmpty();

	/**
 * @param Task&&
 */
	void pushTask(Task &&);

	Task popTask();

	void wakeUpAll();

private:
	size_t             capacity_;
	queue<Task>        que_;
	mutex              mutex_;
	condition_variable isFull_;
	condition_variable isEmpty_;
	bool               isExit_;
};

#endif //_TASKQUEUE_H