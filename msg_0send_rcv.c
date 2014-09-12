#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>


struct msgbuf {
	long m_type;
	char m_data[256];
};

int main()
{
	int mqid; // message queue id
	key_t msgkey;
	struct msgbuf msgdata, *p;

	// send
	p = &msgdata;
	p->m_type = getpid();
	msgkey = ftok("mrecv", 'a');
	mqid = msgget(msgkey, IPC_CREAT | 0666);
	strcpy(p->m_data, "test1");
	msgsnd(mqid, p, sizeof(p->m_data), 0);
	strcpy(p->m_data, "test2");
	msgsnd(mqid, p, sizeof(p->m_data), 0);
	strcpy(p->m_data, "test3");
	msgsnd(mqid, p, sizeof(p->m_data), 0);

	// recv
	msgkey = ftok("mrecv", 'a');
	mqid = msgget(msgkey, IPC_CREAT | 0666);
	while(1) {
		msgrcv(mqid, p, sizeof(p->m_data), 0, 0);
		printf("message %d %s\n", p->m_type, p->m_data);
	}
}