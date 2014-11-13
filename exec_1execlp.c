#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv)
{
	execlp("ls", "ls", "-l", NULL);

	printf("DBG > 7\n"); // does not come here unless error

	return 0;
}
