#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h> // for EPERM

/*
gcc [file] -lpthread
*/

int main(void)
{
    int ret;
    struct sched_param param_set;
    struct sched_param param_get;
    pthread_t this_thread;
    int policy;

    this_thread = pthread_self();

    // before
    ret = pthread_getschedparam(this_thread, &policy, &param_get);
    if (ret != 0)
    {
		printf("pthread_getschedparam : failed\n");
        return 0;
    }
    printf ("before: policy=%ld, priority=%ld\n", policy, param_get.sched_priority);


    // highest
    param_set.sched_priority = sched_get_priority_max(SCHED_FIFO);
    printf("highest priority = %d\n", param_set.sched_priority);

    // set
    ret = pthread_setschedparam(this_thread, SCHED_FIFO, &param_set);
    if (ret == EPERM) {
        printf("pthread_setschedparam : failed because non-root user\n");
		return 0;    	
    }
    if (ret != 0)
    {
        printf("pthread_setschedparam : failed\n");
        return 0;
    }

    // after
    ret = pthread_getschedparam(this_thread, &policy, &param_get);
    if (ret != 0)
    {
		printf("pthread_getschedparam : failed\n");
        return 0;
    }
    printf ("after: policy=%ld, priority=%ld\n", policy, param_get.sched_priority);

    return 0;
}
