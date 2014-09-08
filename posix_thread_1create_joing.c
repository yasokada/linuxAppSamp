#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
http://blog.majide.com/2009/03/thread-sample-program/
*/

/*
gcc [file] -lpthread
*/

void *print_message_function( void *ptr );

int main(void)
{
	pthread_t thread1, thread2;
	char *message1 = "Thread 1";
	char *message2 = "Thread 2";
	int iret1, iret2;

	/* Create independent threads each of which will execute function */

	iret1 = pthread_create( &thread1, NULL, print_message_function, (void*)message1);
	iret2 = pthread_create( &thread2, NULL, print_message_function, (void*)message2);

	pthread_join( thread1, NULL );
	pthread_join( thread2, NULL );
	exit(0);
}

void *print_message_function( void *ptr )
{
	char *message;
	message = (char *)ptr;
	printf("%s \n", message);
}

