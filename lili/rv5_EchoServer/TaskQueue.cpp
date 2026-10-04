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
bool TaskQueue::pushTask(Task &&task)
{
	if(isExit_) {
		return false;
	}

	{
		unique_lock<mutex> ul(mutex_);

		while(capacity_ == que_.size()) { //is full

			if(isExit_) {
				return false;
			}

			isFull_.wait(ul);
		}

		que_.push(std::move(task));
	}

	isEmpty_.notify_one(); //唤醒 阻塞在is empty上的线程
	return true;
}

/**
 * @return Task
 */
Task TaskQueue::popTask()
{
	Task task = nullptr;
	{
		unique_lock<mutex> ul(mutex_);

		while(0 == que_.size()) //is empty
		{
			if(isExit_) {
				return nullptr;
			}
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
	//8. isExit_ 无锁写存在数据竞争；且只唤醒 isEmpty_，队列满时阻塞在 isFull_ 上的生产者永远卡死
	//修改前的代码：
	/*
	isExit_ = true;
	isEmpty_.notify_all();
	*/
	//修改过后的代码：
	isExit_ = true;
	isEmpty_.notify_all();
	isFull_.notify_all();
}