#include <stdio.h>
#include <stdbool.h> // for bool
#include <sys/mount.h> // for mount(2)
#include <sys/stat.h> // for mkdir(2)
#include <fcntl.h> // for O_RDWR
#include <string.h>

const char *kMountPos = "/media/card";

static void write_sample(void)
{
	int fd;
	char szBuf[100];

	memset(szBuf, 0, sizeof(szBuf));
	strcpy(szBuf, kMountPos);
	strcat(szBuf, "/test.csv");
	printf("file name = %s\n", szBuf);
	fd = open(szBuf, O_CREAT | O_APPEND | O_WRONLY, 0666);

	if (fd < 0) {
		printf("open fail\n");
		return;
	}

	sprintf(szBuf, "test,1\r\n");
	write(fd, szBuf, strlen(szBuf));
	write(fd, szBuf, strlen(szBuf));
	write(fd, szBuf, strlen(szBuf));

	close(fd);
}

static bool sd_mount(void)
{
	int ret;

	ret = mkdir(kMountPos, 0777); 
	// no check for mkdir

	ret = mount("/dev/mmcblk0p1", kMountPos, "vfat",  MS_MGC_VAL | MS_SYNCHRONOUS, NULL);
	if (ret == 0) {
		return true;
	}
	return false;
}

static bool sd_umount(void)
{
	int ret;

	ret = umount(kMountPos);
	if (ret == 0) {
		return true;
	}
	return false;
}

int main(void)
{
	if (sd_mount()) {
		printf("sd mounted to %s\n", kMountPos);
	} else {
		printf("sd mount : fail\n");		
	}

	printf("writing sample data\n");
	write_sample();
	sleep(1);

	if (sd_umount()) {
		printf("sd umounted %s\n", kMountPos);
	} else {
		printf("sd umount : fail\n");
	}

}
