#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*
TODO: two warnings related to (void*)
*/

void *testFunc(void *arg)
{
	int loop;
	int thr_idx;

	thr_idx = *(int*)arg;
	printf("Thread %d\n", thr_idx);

	free(arg);
	return NULL;
}

const int kNum = 10;

int main(void)
{
	int loop;
	pthread_t thr[kNum];
	int *pint; // for passing argument

	for(loop=0; loop<kNum; loop++) {
		pint = malloc(sizeof(*pint));
		*pint = loop;
		pthread_create(&thr[loop], NULL, testFunc, pint);
	}
	for(loop=0; loop<kNum; loop++) {
		pthread_join(thr[loop], NULL);
	}

	return 0;
}
