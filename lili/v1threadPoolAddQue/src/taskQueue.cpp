/**
 * Project Untitled
 */

#include "taskQueue.h"

/**
 * taskQueue implementation
 */

/**
 * @return void
 */
taskQueue::taskQueue(int size)
    : queSize_(size)
    , que_()
    , mutex_()
    , notFull_()
    , notEmpty_()
{
	return;
}

/**
 * @return bool
 */
bool taskQueue::isFull()
{
	return queSize_ == curSize();
}

/**
 * @return bool
 */
bool taskQueue::isEmpty()
{
	return 0 == curSize();
}

/**
 * @return int
 */
int taskQueue::curSize()
{
	return que_.size();
}

/**
 * @return void
 */
// void taskQueue::pushElem(int elem) {

// 	unique_lock<mutex> ul(mutex_);
//     //如果不满就放任务，通知有任务
//     if(!isFull()){
// 		que_.push(elem);
// 		ul.unlock();
// 		notEmpty_.notify_one();
// 	} else { //否则休眠 (这样写的话，如果休眠之后被唤醒了，是不是就不会放休眠之前要存放的任务呢？)
// 		notFull_.wait(ul);
// 	}
// }

//是的，上面是存在我说的那个问题，deepseek 版本
void taskQueue::pushElem(int elem)
{
	unique_lock<mutex> ul(mutex_);
	notFull_.wait(ul, [this] { return !isFull(); }); // 等价于 while(!isFull()) wait
	que_.push(elem);
	notEmpty_.notify_one();
}

/**
 * @return int
 */
int taskQueue::popElem()
{

	unique_lock<mutex> ul(mutex_);
	if(isEmpty()) { //如果空，就休眠 等待notEmpty时被唤醒
		notEmpty_.wait(ul);
	}
	// int elem = que_.back(); back取的是最后一个元素，queue pop的是第一个元素，使用把back改成front
	int elem = que_.front();
	que_.pop();
	notFull_.notify_one();
	return elem;
}

/**
 * @return void
 */
taskQueue::~taskQueue()
{
	return;
}