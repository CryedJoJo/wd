#include <stdio.h>
#include <threads.h>

//threadPool
typedef struct{
	int capacity_;
	int taskQueueSize_;
	int queueFront_;
	int queueRear_;


} threadPool;

typedef struct{
	int clientId_;
	int netFd_;
	(void *)(void *function)(void *);
	void *arg_;
} task;