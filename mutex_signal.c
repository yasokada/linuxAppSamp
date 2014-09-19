#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
[result]
main > locked
main > created
thread > locked
thread > signal
main > signaled
main > unlocked
thread > unlocked
*/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *threadproc(void *data) {
	pthread_mutex_lock(&lock);
	printf("thread > locked\n");
	pthread_cond_signal(&cond);
	printf("thread > signal\n");
	pthread_mutex_unlock(&lock);
	printf("thread > unlocked\n");
}

int main(int argc, char *argv[]) {
	pthread_t thread;
	pthread_mutex_lock(&lock);
	printf("main > locked\n");
	pthread_create(&thread, NULL, &threadproc, NULL);
	printf("main > created\n");

	pthread_cond_wait(&cond, &lock);
	printf("main > signaled\n");

	pthread_mutex_unlock(&lock);
	printf("main > unlocked\n");
	pthread_join(thread, NULL);
	return 0;
}
