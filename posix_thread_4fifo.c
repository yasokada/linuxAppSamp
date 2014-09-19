#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <ctype.h>
#include <unistd.h>

pthread_mutex_t s_m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s_m2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  s_c1 = PTHREAD_COND_INITIALIZER;

const int kNumThread = 3;
const int kLoopCount = 10000000;

void *thread_func( void *arg );

static double toSec(struct timeval tv) { return tv.tv_sec + tv.tv_usec / 1e6; }

int main( int argc, char ** argv ) 
{
    struct sched_param thr_param;
    pthread_t thr_id[kNumThread];
    pthread_attr_t thr_attr;
    int policy = 0;
    double *pIntvl[kNumThread];
    int prio_min = 50;
    int prio_max = 50;
    int ret = 0;
    int loop;

    pthread_attr_init( &thr_attr );
    pthread_attr_setinheritsched( &thr_attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setschedpolicy( &thr_attr, SCHED_FIFO );
    prio_min = sched_get_priority_min( SCHED_FIFO );
    prio_max = sched_get_priority_max( SCHED_FIFO );

    pthread_mutex_lock( &s_m2 );
    pthread_mutex_lock( &s_m1 );
    for( loop=0; loop < kNumThread; loop++ ) {
        thr_param.sched_priority = prio_min + (10 * loop); 
        pthread_attr_getschedpolicy( &thr_attr, &policy );
        printf( " %d priority [%d - %d] %d\n",
            loop, prio_min, prio_max, thr_param.sched_priority );
        pthread_attr_setschedparam( &thr_attr, &thr_param );
        ret = pthread_create( &thr_id[loop], &thr_attr, thread_func, (void *)&loop );
        if( ret != 0) {
            printf( "pthread_create : Failed [%d][%s]\n", ret, strerror(ret));
            return 1;
        }
        pthread_cond_wait( &s_c1, &s_m1 );
    }
    pthread_mutex_unlock( &s_m1 );
    pthread_mutex_unlock( &s_m2 );

    for( loop=0; loop < kNumThread; loop++ ) {
        pthread_join( thr_id[loop], (void **)&pIntvl[loop] );
    }
    for( loop=0; loop < kNumThread; loop++ ) {
        printf( " %d , priority %d timesec:[%f]\n",
            loop, thr_param.sched_priority, *pIntvl[loop] );
    }

    for( loop=0; loop < kNumThread; loop++ ) {
        free(pIntvl[loop]); // free those malloced in thread_func()
    }
    return 0;
}

void *thread_func( void *arg ) 
{
    int index;
    double *pret; // to return (void *)
    struct timeval tv1;
    struct timeval tv2;
    int count, loop;

    index = *(int *)arg;
    pret = malloc(sizeof(double));

    pthread_mutex_lock(&s_m1);
    pthread_cond_signal(&s_c1);
    pthread_mutex_unlock(&s_m1);

    gettimeofday(&tv1, 0);
    pthread_mutex_lock(&s_m2);
    pthread_mutex_unlock(&s_m2);

    count = 0;
    for(loop=0; loop < kLoopCount; loop++) {
        count++;
        if(count % 1000 == 0) {
            printf(" %d %.*s%4d\r",index, index + 1, "\t\t\t", count / 1000 );
        }
    }
    gettimeofday(&tv2, 0);
    printf(" %d fin\n", index );
    *pret = toSec(tv2) - toSec(tv1);

    return (void *)pret;
}

