/**
 * Project Untitled
 */

#include "TaskQueue.h"

/**
 * TaskQueue implementation
 */

TaskQueue::TaskQueue(size_t capacity)
    : capacity_(capacity)
    , que_()
    , mutex_()
    , isFull_()
    , isEmpty_()
    , isExit_(false)
{
}

TaskQueue::~TaskQueue()
{
}

/**
 * @return bool
 */
bool TaskQueue::isEmpty()
{
	unique_lock<mutex> ul(mutex_);
	return 0 == que_.size();
}

/**
 * @param Task&&
 * @return void
 */
void TaskQueue::pushTask(Task &&task)
{
	{
		unique_lock<mutex> ul(mutex_);

		while(capacity_ == que_.size()) { //is full

			if(isExit_) {
				return;
			}

			isFull_.wait(ul);
		}

		que_.push(std::move(task));
	}

	isEmpty_.notify_one(); //唤醒 阻塞在is empty上的线程
}

/**
 * @return Task
 */
Task TaskQueue::popTask()
{
	Task task;
	{
		unique_lock<mutex> ul(mutex_);

		while(0 == que_.size()) //is empty
		{
			isEmpty_.wait(ul);
		}

		task = std::move(que_.front());
		que_.pop();
	}
	isFull_.notify_one(); //唤醒阻塞在 is full线程
	return task;
}

/**
 * @return void
 */
void TaskQueue::wakeUpAll()
{
	isExit_ = true;
	isEmpty_.notify_all();
}