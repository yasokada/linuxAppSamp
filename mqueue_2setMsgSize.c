	#include <stdio.h>
	#include <mqueue.h> // for message queue
	#include <sys/stat.h>
	#include <stdlib.h> // for EXIT_FAILURE
	#include <string.h>

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

	int main()
	{
		mqd_t mqd;
		int flags;
		int ret;
		struct mq_attr attr;

		flags = O_RDWR | O_CREAT;

		attr.mq_flags = 0; // or O_NONBLOCK
		attr.mq_maxmsg = 60;
		attr.mq_msgsize = 120;
		attr.mq_curmsgs = 0;

		// POSIX IPC name should start with "/"
		mqd = mq_open("/mq", flags,
	//		(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH),
			0644,
			&attr );

		if (mqd < 0) {
			printf("open failed\n");
			exit(EXIT_FAILURE);
		}
		printf("open ok\n");

		sleep(1);

		showAttr(mqd);

		ret = mq_close(mqd);
		if (ret != 0) {
			printf("open failed\n");
			exit(EXIT_FAILURE);		
		}
		printf("close ok\n");

		return 0;
	}