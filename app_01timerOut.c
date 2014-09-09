#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for O_WRONLY / O_RDONLY
#include <pthread.h>
#include <stdbool.h> // for bool
#include <time.h> // for time_t, tm

// IO
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define IN_GPIO (51) // for MicroZed
#define OUT_GPIO (47) // for MicroZed

#define MAX_BUF (100)

#define RET_OK (0)

static char *kVersion = "V0.2";

typedef enum {
	DIR_IN = 0,
	DIR_OUT = 1
} enum_DIR;

static int gpio_export(int gpio) 
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		return fd; // error
	}
	printf("setting export ...\n");
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);

	return 0;
}

static int gpio_set_direction(int gpio, enum_DIR enum_dir)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%2d/direction", gpio);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		return fd; // error
	}
	printf("setting direction ...\n");
	if (enum_dir == DIR_OUT) {
		strcpy(buf, "out");
	} else {
		strcpy(buf, "in");		
	}
	write(fd, buf, strlen(buf));
	close(fd);

	return 0;
}

static int gpio_set_value(int gpio, int hi)
{
	int fd, len;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%2d/value", gpio);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		return fd; // error
	}
	len = snprintf(buf, sizeof(buf), "%d", hi);
	write(fd, buf, len);
	close(fd);

	return 0;
}

static bool gpio_get_hiLow(int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
	char code;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%2d/value", gpio);
	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		return fd; // error
	}
	read(fd, &code, 1);
	close(fd);

	if (code == '1') {
		return 1;		
	} else {
		return 0;
	}
}

//-----------------------------------
static bool s_bfStart = false;
//-----------------------------------

void *ledTask(void *ptr)
{
	printf("ledTask started\n");

	if (gpio_export(OUT_GPIO) != RET_OK) {
		printf("ledTask > gpio not found\n");
		return;
	}
	if (gpio_set_direction(OUT_GPIO, DIR_OUT) != RET_OK) {
		printf("ledTask > gpio dir set fail\n");
		return;
	}

	while(1) {
	    if (s_bfStart) { 
			gpio_set_value(OUT_GPIO, 1);
		} else {
			gpio_set_value(OUT_GPIO, 0);
		}
		usleep(100000);
	}
}

void *swTask(void *ptr)
{
	int ret;
	char buf[MAX_BUF];
	bool hist[3] = { false, false, false }; // old..new
	bool now;
	int pos = 0;

	printf("swTask started\n");

	if (gpio_export(IN_GPIO) != RET_OK) {
		printf("swTask > gpio not found\n");
		return;
	}
	if (gpio_set_direction(IN_GPIO, DIR_IN) != RET_OK) {
		printf("swTask > gpio dir set fail\n");
		return;
	}

	while(1) {
		usleep(20000); // 20 msec

//		printf(".");
//		fflush(stdout);

		now = gpio_get_hiLow(IN_GPIO);
		if (pos < 3) {
			hist[0] = hist[1];
			hist[1] = hist[2];
			hist[2] = now;
			pos++;
			continue;
		}

		if ( (! hist[0] && ! hist[1] && hist[2] && now) ) {
		    s_bfStart = !s_bfStart;
		}
		hist[0] = hist[1];
		hist[1] = hist[2];
		hist[2] = now;
	}

	// TODO: unexport

}

static int getDateTimeStr(char *dest)
{
	time_t t;
	struct tm tm;
	int len;

	if (dest == NULL) {
		return 0;
	}

	t = time(NULL);
	tm = *localtime(&t);

	len = sprintf(dest, "%04d/%02d/%02d %02d:%02d:%02d",
			tm.tm_year + 1900, 
			tm.tm_mon + 1, // tm_mon : since January
			tm.tm_mday,
			tm.tm_hour,
			tm.tm_min,
			tm.tm_sec
		);
	if (len == EOF) {
		return 0;
	}
	return len;		
}

void *mainTask(void *ptr)
{
	char szBuf[100];

	while(1) {
		if (s_bfStart) {
			if (getDateTimeStr(szBuf) > 0) {
				printf("%s\n", szBuf);
			}
		}
		usleep(1000000);  // TODO: how to get strict 1 sec interval
	}
}

int main(void)
{
	pthread_t swThread, ledThread, mainThread;
	int retSw, retLed, retMain;

	printf("----------------------\n");
	printf("Hello world %s\n", kVersion);

	retSw  = pthread_create( &swThread, NULL, swTask, NULL);
	retLed = pthread_create( &ledThread, NULL, ledTask, NULL);
	retMain = pthread_create( &mainThread, NULL, mainTask, NULL);

	pthread_join( swThread, NULL );
	pthread_join( ledThread, NULL );
	pthread_join( mainThread, NULL );
	exit(0);
}
