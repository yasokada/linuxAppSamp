#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void funcWrite(const char *msg, int cnt, FILE *stream)
{
	int loop;

	for(loop=0; loop<cnt; loop++) {
		fprintf(stream, "%s\n", msg);
		fflush(stream);
		sleep(1);
	}
}

static void funcRead(FILE *stream)
{
	char zbuf[512];

	while( !feof(stream) && !ferror(stream)
	&& fgets(zbuf, sizeof(zbuf), stream) != NULL ) {
		printf("%s", zbuf);
	    fflush(stdout);
	}
}

int main(void)
{
	int fds[2];
	pid_t pid;
	FILE *stream;

	printf("start\n");

	pipe(fds);

	pid = fork(); /***/
	printf("after fork\n");

	if (pid == (pid_t) 0) { // child
		close(fds[1]);
		stream = fdopen(fds[0], "r");
		funcRead(stream);
		close(fds[0]);
	} else { // parent
		close(fds[0]);
		stream = fdopen(fds[1], "w");
		funcWrite("message from parent", 3, stream);
		close(fds[1]);
	}

	return 0;
}
