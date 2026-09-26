#include "threadPool.h"
#include "myTask.h"
#include <string>

void test()
{

	threadPool pool(3, 10);
	pool.start();

	myTask t1, t2, t3;
	pool.addTask(&t1);
	pool.addTask(&t2);
	pool.addTask(&t3);

	pool.stop();
	std::string s = pool.isExit() ? "yes" : "no";
	std::cout << "is pool exit?" << s << std::endl;
}

int main()
{
	test();

	return 0;
}