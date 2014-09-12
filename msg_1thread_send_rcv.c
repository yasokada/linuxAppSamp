#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

/*
gcc [file] -lpthread
*/

void *send_func(void *ptr);
void *recv_func(void *ptr);

struct msgbuf {
	long m_type;
	char m_data[256];
};

int main(void)
{
	pthread_t thread1, thread2;
	int iret1, iret2;

	iret1 = pthread_create( &thread1, NULL, send_func, (void*)NULL);
	iret2 = pthread_create( &thread2, NULL, recv_func, (void*)NULL);

	pthread_join( thread1, NULL );
	pthread_join( thread2, NULL );

	return 0;
}


void *send_func(void *ptr)
{
	int mqid;
	key_t msgkey;
	struct msgbuf msgdata;
	struct msgbuf *p;
	int loop;

	printf("start send thread\n");

	p = &msgdata;
	p->m_type = getpid();
	msgkey = ftok("mrecv", 'a');
	mqid = msgget(msgkey, IPC_CREAT | 0666);

	for(loop=0; loop<5; loop++) {
		strcpy(p->m_data, "test1");
		printf("  send message\n");
		msgsnd(mqid, p, sizeof(p->m_data), 0);
	}

	printf("end send thread\n");

}

void *recv_func(void *ptr)
{
	int mqid;
	key_t msgkey;
	struct msgbuf msgdata;
	struct msgbuf *p;
	struct msqid_ds info;

	sleep(1);

	printf("start recv thread\n");

	p = &msgdata;
	msgkey = ftok("mrecv", 'a');
	mqid = msgget(msgkey, IPC_CREAT | 0666);

	while(1) {
		msgctl(mqid, IPC_STAT, &info);
		if (info.msg_qnum == 0) { // no message
			usleep(100000);
			continue;
		}

		msgrcv(mqid, p, sizeof(p->m_data), 0, 0);
		printf("recv %d %s in %d msgs\n", p->m_type, p->m_data);
		usleep(500000);
	}

	printf("end recv thread\n");
}

