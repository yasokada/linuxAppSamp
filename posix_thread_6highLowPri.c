#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h> // for EPERM

void *thread_HighPri(void *ptr);
void *thread_lowPri(void *ptr);

static void s_wait_without_sleep(int sec);

/*
Thread priority test

- blocks low-priority thread if USE_SLEEP is undefined
*/

//#define USE_SLEEP // for sleep() instead of s_wait_without_sleep()


int main(void)
{
	pthread_t thread1, thread2;
	char *msg1 = "Thread High created";
	char *msg2 = "Thread Low  created";
	int ret1,  ret2;

	ret1 = pthread_create( &thread1, NULL, thread_HighPri, (void *)msg1);
	ret2 = pthread_create( &thread2, NULL, thread_lowPri, (void *)msg2);

	sleep(20);
}

static const s_kPriHigh = 20;
static const s_kPriLow  = 10; // 10, 20 : arbitrary but for High > Low

static signed int s_setPriority(pthread_t thrd, int pri) 
{
	struct sched_param param_set;
	int ret;

	param_set.sched_priority = pri;
	ret = pthread_setschedparam(thrd, SCHED_FIFO, &param_set);
	if (ret == EPERM) {
		printf("priority set: fail\n");
		return -1;
	}

	printf("priority set: OK\n");
	return 0;	
}

static void s_wait_without_sleep(int sec)
{

#ifdef USE_SLEEP
	sleep(sec);
	return;
#endif

	// Rough wait
    // 
	// does not use sleep() to avoid thread switching
	struct timeval tv1, tv2;
	volatile int loop;

	gettimeofday(&tv1, NULL);
	while(1) {
		gettimeofday(&tv2, NULL);
		if (tv2.tv_sec - tv1.tv_sec >= sec) {
			break;
		}
		for(loop = 0; loop < 10000L; loop++) {
			// do nothing
		}		
	}
}

void *thread_HighPri(void *ptr)
{
	pthread_detach(pthread_self());
	s_setPriority(pthread_self(), s_kPriHigh);

	char *msg;
	msg = (char *)ptr;
	printf("%s \n", msg);

	int loop;
	for(loop = 0; loop < 3; loop++) {
		s_wait_without_sleep(5);
		printf("1 > \n");
	}
}

void *thread_lowPri(void *ptr)
{
	pthread_detach(pthread_self());
	s_setPriority(pthread_self(), s_kPriLow);

	char *msg;
	msg = (char *)ptr;
	printf("%s \n", msg);

	int loop;
	for(loop = 0; loop < 15; loop++) {
		s_wait_without_sleep(1);
		printf("2 > \n");
	}
}


