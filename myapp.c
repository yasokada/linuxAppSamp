#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for O_WRONLY

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF (64)
#define IN_GPIO (51) // for MicroZed
#define OUT_GPIO (47) // for MicroZed

static int gpio_export(int gpio) 
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
//		printf("gpio not found\n");
		return fd; // error
	}
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);

	return 0;
}

int main()
{
	printf("Hello world\n");

	if (gpio_export(OUT_GPIO) != 0) {
		printf("gpio not found\n");
	}

}
