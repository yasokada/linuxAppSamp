#include <signal.h>
#include <unistd.h>

void funcHandler(int signo)
{
	char zbuf[] = "Catch\n";
	write(1, zbuf, sizeof(zbuf));
}

int main(void)
{
	signal(SIGINT, funcHandler);
	raise(SIGINT);

	return 0;
}