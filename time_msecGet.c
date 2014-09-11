#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	struct timeval tv;
	struct tm *tmp;
//	time_t tim;
	int msec;

	gettimeofday(&tv, NULL);
	tmp = localtime(&tv.tv_sec);
//	tim = mktime(tmp);
	msec = tv.tv_usec / 1000;

	printf("%04d/%02d/%02d %02d:%02d:%02d.%03d\n", 
		tmp->tm_year + 1900,
		tmp->tm_mon + 1,
		tmp->tm_mday,
		tmp->tm_hour,
		tmp->tm_min,
		tmp->tm_sec,
		msec
		);	

	return 0;
}