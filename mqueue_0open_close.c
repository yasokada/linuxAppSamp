#include <stdio.h>
#include <mqueue.h> // for message queue
#include <sys/stat.h>
#include <stdlib.h> // for EXIT_FAILURE

/*
gcc [file] -lrt
*/

/* 
To use POSIX message queue, followings should be activated
- CONFIG_POSIX_MQUEUE = y
- CONFIG_POSIX_MQUEUE_SYSCTL = y
*/

int main(int argc, char *argv[])
{
	mqd_t mqd;
	int flags;
	int ret;

	flags = O_RDWR | O_CREAT;

	// POSIX IPC name should start with "/"
	// 
	// without NULL, mq_open() fails on PetaLinux
	mqd = mq_open("/mq", flags,
		(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), NULL );
	if (mqd < 0) {
		printf("open failed %d\n", mqd);
		exit(EXIT_FAILURE);
	}
	printf("open ok\n");

	sleep(1);

	ret = mq_close(mqd);
	if (ret != 0) {
		printf("open failed\n");
		exit(EXIT_FAILURE);		
	}
	printf("close ok\n");

	return 0;
}