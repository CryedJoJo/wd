#include "taskQueue.h"
#include "producer.h"
#include "consumer.h"

#include <thread>
using std::thread;

int main()
{
	taskQueue tq(10);

	producer p;
	consumer c;

	thread t1(&producer::addTask, &p, std::ref(tq));
	thread t2(&consumer::getTask, &c, std::ref(tq));

	t1.join();
	t2.join();

	return 0;
}