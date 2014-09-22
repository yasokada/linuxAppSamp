#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h> // to avoid warning: incompatible implicit declaration...
#include <semaphore.h>

int g_cnt = 0;
sem_t *g_psem;

void *testFunc(void *arg)
{
	int loop;
	int thr_idx;
	int semval;

	thr_idx = *(int*)arg;

	for(loop=0; loop<5; loop++) {
		sem_getvalue(g_psem, &semval);
		if (semval == 0) {
			printf("blocked\n");
		}
		sem_wait(g_psem); /***semaphore***/
		g_cnt++;
		printf("g_cnt : %d > %d\n", thr_idx, g_cnt);
		usleep(100000);
		sem_post(g_psem); /***semaphore***/
//		usleep(100000);
	}

	free(arg);
	return NULL;
}

const char *kSemName = "/sem";
const int kNum = 10;

int main(void)
{
	int loop;
	pthread_t thr[kNum];
	int *pint; // for passing argument

	sem_unlink(kSemName); // just in case

	g_psem = sem_open(kSemName, O_CREAT, 
		(S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH), 1); /***semaphore***/

	for(loop=0; loop<kNum; loop++) {
		pint = malloc(sizeof(*pint));
		*pint = loop;
		pthread_create(&thr[loop], NULL, testFunc, pint);
	}
	for(loop=0; loop<kNum; loop++) {
		pthread_join(thr[loop], NULL);
	}
	printf("\n");

	sem_close(g_psem); /***semaphore***/
	sem_unlink(kSemName); /***semaphore***/

	return 0;
}
