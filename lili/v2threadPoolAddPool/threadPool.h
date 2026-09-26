/**
 * Project Untitled
 */


#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "taskQueue.h" //组合关系不能只前向声明，需要包含头文件

#include <thread>
#include <vector>

using std::vector;
using std::thread;

// class taskQueue;
class Task;

class threadPool {
public: 
    
/**
 * @param size
 */
	threadPool(size_t threadCount, size_t queSize);

	~threadPool(){}

	void init();

	bool isExit();

	bool addTask(Task* task);

	void start();

	void stop();
    

private:
	size_t         threadCount_;
	vector<thread> threads_;
	taskQueue      taskQue_; //不允许使用不完整的类型 组合关系必须包含头文件，这样threadPool的大小才是可以计算的，如果是关联关系，可以只进行前向声明
	bool isExit_;
    
    Task* getTask();
    
    void doTask();
};

#endif //_THREADPOOL_H