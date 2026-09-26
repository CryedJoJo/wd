/**
 * Project Untitled
 */


#include "producer.h"
#include "taskQueue.h"

/**
 * producer implementation
 */


/**
 * @param taskQue
 */
void producer::addTask(taskQueue& taskQue) {

	int i = 20;
	while(i--){
		taskQue.pushElem(i);
		std::cout << "producer add task " << i << std::endl;
	}
}