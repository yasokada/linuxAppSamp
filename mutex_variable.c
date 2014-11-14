#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
gcc [file] -lpthread
*/

void *thread_func1(void *ptr);
void *thread_func2(void *ptr);

static int s_val=0;
pthread_mutex_t lock; /***/

int main(void)
{
	pthread_t thread1, thread2;
	int iret1, iret2;

	iret1 = pthread_create( &thread1, NULL, thread_func1, (void*)NULL);
	iret2 = pthread_create( &thread2, NULL, thread_func2, (void*)NULL);

	pthread_join( thread1, NULL );
	pthread_join( thread2, NULL );

	pthread_mutex_destroy(&lock);

	return 0;
}

static void set_val(int val) {	s_val = val; }
static int get_val(void) { return s_val; }

const int kIntvl_usec = 100000;

void *thread_func1(void *ptr)
{
	int loop;

	usleep(kIntvl_usec);

	for(loop=0; loop<10; loop++) {
		pthread_mutex_lock(&lock); /***/
		set_val(get_val() + 1);
		pthread_mutex_unlock(&lock); /***/
		usleep(kIntvl_usec * 3);
	}
}

void *thread_func2(void *ptr)
{
	int loop;

	for(loop=0; loop<10; loop++) {
		pthread_mutex_lock(&lock);  /***/
		printf("thread 2: %d\n", get_val() );
		usleep(kIntvl_usec);
		pthread_mutex_unlock(&lock); /***/
	}

	for(loop=0; loop<20; loop++) {
		printf("thread 2: %d\n", get_val() );
		usleep(kIntvl_usec);
	}
}

