#include <stdio.h>
#include <stdbool.h> // for bool
#include <sys/mount.h> // for mount(2)
#include <sys/stat.h> // for mkdir(2)

const char *kMountPos = "/media/card";

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

	sleep(1);

	if (sd_umount()) {
		printf("sd umounted %s\n", kMountPos);
	} else {
		printf("sd umount : fail\n");
	}

}
