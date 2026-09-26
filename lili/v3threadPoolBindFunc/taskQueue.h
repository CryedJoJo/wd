/**
 * Project Untitled
 */

#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::queue;

using ElemType = std::function<void()>;

class taskQueue {
public:
	/**
 * @param size
 */
	taskQueue(size_t size);
	~taskQueue() {}

	size_t curSize();

	/**
 * @param task
 */
	bool pushElem(ElemType &&task);

	ElemType popElem();

	void wakeupAll();

	

private:
	bool isFull();
	bool isEmpty();
	void exitQ();

	size_t             queSize_;
	mutex              mutex_;
	condition_variable notFull_;
	condition_variable notEmpty_;
	queue<ElemType>    que_;
	bool               exitFlag_;
};

#endif //_TASKQUEUE_H