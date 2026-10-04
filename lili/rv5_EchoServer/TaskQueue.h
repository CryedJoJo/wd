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
#include <atomic>

using std::function;
using std::queue;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::atomic;
using Task = std::function<void()>;

class TaskQueue {
public:
	TaskQueue(size_t capacity);

	~TaskQueue();

	bool isEmpty();

	/**
 * @param Task&&
 */
	bool pushTask(Task &&);

	Task popTask();

	void wakeUpAll();

private:
	size_t             capacity_;
	queue<Task>        que_;
	mutex              mutex_;
	condition_variable isFull_;
	condition_variable isEmpty_;
	//8. isExit_ 在 pushTask 无锁读、wakeUpAll 无锁写，数据竞争
	//修改前的代码：
	// bool isExit_;
	//修改过后的代码：
	atomic<bool>       isExit_;
};

#endif //_TASKQUEUE_H