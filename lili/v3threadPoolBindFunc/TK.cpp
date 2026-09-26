/**
 * Project Untitled
 */

#include "TK.h"
#include <iostream>

/**
 * TK implementation
 */

/**
 * @return void
 */
void TK::process()
{
	static int i = 20;
	while(i--) {
		std::cout << "proc " << i << "is working..." << std::endl;
	}
}