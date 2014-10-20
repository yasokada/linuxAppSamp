#include <stdio.h>
#include <time.h>
#include <sys/reboot.h>

int main(void){
	time_t t;
	struct tm tm;
	int loop;

	printf("Hellow World\n");

	for  (loop=0; loop<20; loop++) {
		t = time(NULL);
		tm = *localtime(&t);

		printf("now: %d-%d-%d %d:%d:%d\n",
			tm.tm_year + 1900, 
			tm.tm_mon + 1, // tm_mon : since January
			tm.tm_mday,
			tm.tm_hour,
			tm.tm_min,
			tm.tm_sec
			);
		sleep(1);
	}

	reboot(RB_AUTOBOOT);
}