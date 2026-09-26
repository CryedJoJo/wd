/**
 * Project Untitled
 */


#include "myTask.h"

#include <iostream>

/**
 * myTask implementation
 */


/**
 * @return void
 */
void myTask::process() {
	static int i = 0;
	std::cout << "processing " <<  i++ << std::endl;
}