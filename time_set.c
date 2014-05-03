#include <errno.h>
#include <stdio.h>
#include <time.h> // for mktime()
#include <sys/time.h> // for settimeofday()

int main(void)
{
	int res;

	struct tm tmWork;
	struct timeval timevalWork;

	tmWork.tm_year = 2014-1900;
	tmWork.tm_mon = (5-1); // since January
	tmWork.tm_mday = 3;
	tmWork.tm_hour = 12;
	tmWork.tm_min = 30;
	tmWork.tm_sec = 45;

	timevalWork.tv_sec = mktime(&tmWork);
	timevalWork.tv_usec = 0;

	res = settimeofday(&timevalWork, NULL);

	if (res < 0) {
		printf("set time : fail\n");
	}

	return 0;
}