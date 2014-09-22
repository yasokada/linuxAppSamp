#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

int g_cnt = 0;
sem_t *g_psem;

void *testFunc(void *arg)
{
	int loop;

	for(loop=0; loop<1000; loop++) {
		sem_wait(g_psem); /***/
		g_cnt++;
		printf("g_cnt : %d\r", g_cnt);
		sem_post(g_psem); /***/
	}
	return NULL;
}

const char *kSemName = "/sem";
const int kNum = 100;

int main(void)
{
	int loop;
	pthread_t thr[kNum];

	g_psem = sem_open(kSemName, O_CREAT, 
		(S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH), 1); /***/

	for(loop=0; loop<kNum; loop++) {
		pthread_create(&thr[loop], NULL, testFunc, NULL);
	}
	for(loop=0; loop<kNum; loop++) {
		pthread_join(thr[loop], NULL);
	}
	printf("\n");

	sem_close(g_psem); /***/
	sem_unlink(kSemName); /***/

	return 0;
}
