#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t  cond_sell;
pthread_cond_t  cond_add;
int             num = 0;

void *sell(void *arg)
{
	printf("sell start...\n");

	while(1) {

		pthread_mutex_lock(&lock);
		if(num > 0) {
			printf("selling, ticks suply %d\n", num);
			num--;
			pthread_cond_signal(&cond_add);
		} else if(num == 0) {
			printf("ticks count: %d\n", num);
			pthread_cond_wait(&cond_sell, &lock);
			pthread_cond_signal(&cond_add);
		}
		pthread_mutex_unlock(&lock);
	}
	printf("sell end...\n");
}

void *add(void *arg)
{
	printf("add start...\n");

	while(1) {
		pthread_mutex_lock(&lock);
		if(num < 10) {
			printf("add ticks to: %d\n", ++num);
			pthread_cond_signal(&cond_sell);
		} else if (num == 10){
			printf("add ticks to: %d\n", num);
			pthread_cond_wait(&cond_add, &lock);
			pthread_cond_signal(&cond_sell);
		}
		pthread_mutex_unlock(&lock);
	}
	printf("add end...\n");
}

int main()
{

	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond_sell, NULL);
	pthread_cond_init(&cond_add, NULL);

	pthread_t sell_id;
	pthread_t add_id;
	pthread_create(&sell_id, NULL, sell, NULL);
	pthread_create(&add_id, NULL, add, NULL);

	void *nump;
	pthread_join(sell_id, &nump);
	pthread_join(add_id, &nump);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond_sell);
	pthread_cond_destroy(&cond_add);

	return 0;
}
