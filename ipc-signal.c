#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

static int g_fin = 0; // 1: fin

void funcInt()
{
	char buf[] = "SIGINT:Catch\n";
	write(1, buf, sizeof(buf)); // Do not use printf()
}
void funcHup()
{
	char buf[] = "SIGHUP:Catch\n";
	write(1, buf, sizeof(buf)); // Do not use printf()
	g_fin = 1;
}

int main(void)
{
	pid_t pid;
	int loop;

	pid = fork();

	if (pid == 0) { // child
		signal(SIGINT, funcInt);
		signal(SIGHUP, funcHup);

		while(g_fin == 0) {
			sleep(1);
		}
		printf("child > end\n");
	} else { // parent
		sleep(1);
		for(loop=0; loop<3; loop++) {
			kill(pid, SIGINT);
			sleep(1);
		}
		kill(pid, SIGHUP);
		sleep(1);
		printf("parent > end\n");
	}
}

