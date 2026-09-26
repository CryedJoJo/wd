/**
 * Project Untitled
 */


#include "consumer.h"
#include "taskQueue.h"

/**
 * consumer implementation
 */


/**
 * @param taskQue
 */
void consumer::getTask(taskQueue& taskQue) {

	int i = 20;
	while(i--) {
		std::cout << "consumer get task " << taskQue.popElem() << std::endl;
	}
}