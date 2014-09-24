#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>

// IO
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define IN_GPIO (51) // for MicroZed
#define OUT_GPIO (47) // for MicroZed

#define MAX_BUF (100)

#define RET_OK (0)

static char *kVersion = "V0.1";

typedef enum {
	DIR_IN = 0,
	DIR_OUT = 1
} enum_DIR;

typedef enum {
	EDGE_RISING = 0,
	EDGE_FALLING
} enum_EDGE;

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

static int gpio_set_edge(int gpio, enum_EDGE enum_edge)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%2d/edge", gpio);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		return fd; // error
	}
	printf("setting edge ...\n");
	if (enum_edge == EDGE_RISING) {
		strcpy(buf, "rising");
	} else {
		strcpy(buf, "falling");		
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

static int gpio_get_value(int gpio)
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

int main(void) {
	int ret;
	int epfd;
	int fd;
	struct epoll_event ev1;
	struct epoll_event events;
	int count=0;
	char code;

	// initialization
	if (gpio_export(OUT_GPIO) != RET_OK || gpio_export(IN_GPIO) != RET_OK) {
		printf("gpio not found\n");
		return;
	}
	if (gpio_set_direction(OUT_GPIO, DIR_OUT) != RET_OK || 
		gpio_set_direction(IN_GPIO, DIR_IN) != RET_OK) {
		printf("gpio dir set fail\n");
		return;
	}
	gpio_set_edge(IN_GPIO, EDGE_RISING);


	// example of epoll()
	epfd = epoll_create(1); /*** epoll ***/
	fd = open("/sys/class/gpio/gpio51/value", O_RDONLY);
	if (fd == 0) {
		printf("open fd fail\n");
		close(epfd);
		return;
	}
	read(fd, &code, 1); // need this to make epoll() work

	ev1.events = EPOLLPRI | EPOLLET | EPOLLIN;
	ev1.data.fd = fd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev1); /*** epoll ***/

	while(1) {
		ret = epoll_wait(epfd, &events, 1, -1); /*** epoll ***/
		if (ret == 0) {
			continue;
		}
		if (ret > 0) {
			lseek(fd, 0, SEEK_SET);
			read(fd, &code, 1);
			if (code == '1') {
				printf("pushed\n");
				count++;
				if (count >= 3) {
					break;
				}
			}
		} else {
			printf("ret is nonzero\n");
		}
	}

	close(epfd);
	close(fd);
}
