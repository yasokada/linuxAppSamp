#include <stdio.h>
#include <mqueue.h> // for message queue
#include <sys/stat.h>
#include <stdlib.h> // for EXIT_FAILURE
#include <string.h>

/*
gcc [file] -lrt
*/

static void s_send(mqd_t mqd)
{
	void *ptr;
	unsigned int prio;
	char *msg = "Hellow World";
	int loop;

	prio = 10;

	ptr = calloc(strlen(msg) + 1, sizeof(char));
	strcpy(ptr, msg);
	for (loop=0; loop<3; loop++) {
		printf("  sent > %s\n", msg);
		mq_send(mqd, ptr, strlen(ptr), prio);
	}

	free(ptr);
}

static void s_rcv(mqd_t mqd)
{
	unsigned int prio;
	struct mq_attr attr;
	void *buf;
	int num;
	char *pmsg;
	int loop;

	mq_getattr(mqd, &attr);
	buf = malloc(attr.mq_msgsize);

	printf("maxmsg = %d\n", attr.mq_maxmsg);
	printf("msgsize = %d\n", attr.mq_msgsize);
	printf("curmsgs = %d\n", attr.mq_curmsgs);

	for (loop=0; loop < attr.mq_curmsgs; loop++) {
		num = mq_receive(mqd, buf, attr.mq_msgsize, &prio);
		printf("  num = %d\n", num);
		pmsg = (char *)buf;
		printf("  rcvd > %s\n", pmsg);
	}

	free(buf);
}

int main()
{
	mqd_t mqd;
	int flags;
	int ret;

	flags = O_RDWR | O_CREAT;

	// POSIX IPC name should start with "/"
	mqd = mq_open("/mq", flags,
		(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) );
	if (mqd < 0) {
		printf("open failed\n");
		exit(EXIT_FAILURE);
	}
	printf("open ok\n");

	sleep(1);

	s_send(mqd);

	s_rcv(mqd);

	ret = mq_close(mqd);
	if (ret != 0) {
		printf("open failed\n");
		exit(EXIT_FAILURE);		
	}
	printf("close ok\n");

	return 0;
}