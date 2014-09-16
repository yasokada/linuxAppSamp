#include <stdio.h>
#include <mqueue.h> // for message queue
#include <sys/stat.h>
#include <stdlib.h> // for EXIT_FAILURE
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/*
gcc [file] -lrt
*/

static void showAttr(mqd_t mqd)
{
	struct mq_attr attr;

	mq_getattr(mqd, &attr);

	printf("maxmsg = %d\n", attr.mq_maxmsg);
	printf("msgsize = %d\n", attr.mq_msgsize);
	printf("curmsgs = %d\n", attr.mq_curmsgs);

}

const int kNumMessages = 20;

static void s_send(mqd_t mqd)
{
	void *ptr;
	unsigned int prio;
	char *msg = "Hellow World";
	int loop;

	prio = 10;

	ptr = calloc(strlen(msg) + 1, sizeof(char));
	strcpy(ptr, msg);
	for (loop=0; loop<kNumMessages; loop++) {
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
//		printf("  num = %d\n", num);
		pmsg = (char *)buf;
		printf("%d:  rcvd > %s\n", loop, pmsg);
	}

	free(buf);
}

static bool setMaxMessages(int max_msg)
{
	int fd;
	char szBuf[10];
	const char *msg_max = "/proc/sys/fs/mqueue/msg_max";

	fd = open(msg_max, O_CREAT | O_WRONLY, 0666);

	if (fd < 0) {
//		printf("open fail\n");
		return false;
	}

	sprintf(szBuf, "%d\n", max_msg);
	write(fd, szBuf, strlen(szBuf));

	close(fd);
	return true;
}


int main()
{
	mqd_t mqd;
	int flags;
	int ret;
	struct mq_attr attr;

	flags = O_RDWR | O_CREAT;

	mq_unlink("/mq");

	// following needs root priviledge
	if (setMaxMessages(60) == false) {
		printf("setMaxMessages : failed\n");
		return 0;
	}

	// mq_maxmsg should be less than and equal to the value defined
	// in /proc/sys/fs/mqueue/msg_max
	attr.mq_flags = 0;
	attr.mq_maxmsg = 60;
	attr.mq_msgsize = 256;
	attr.mq_curmsgs = 0;

	mqd = mq_open("/mq", flags,
		(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH),
		&attr );

	if (mqd < 0) {
		printf("open failed %d\n", mqd);
		exit(EXIT_FAILURE);
	}
	printf("open ok\n");

	sleep(1);

	showAttr(mqd);

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