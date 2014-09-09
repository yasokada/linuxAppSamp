#include <stdio.h>
#include <stdbool.h> // for bool
#include <sys/mount.h> // for mount(2)
#include <sys/stat.h> // for mkdir(2)

static bool sd_mount(void)
{
	const char *mountPos = "/media/card";
	int ret;

	ret = mkdir(mountPos, 0777); 
	// no check for mkdir

	ret = mount("/dev/mmcblk0p1", mountPos, "vfat",  MS_MGC_VAL | MS_SYNCHRONOUS, NULL);
	if (ret == 0) {
		return true;
	} else {
		return false;
	}
}

int main(void)
{
	if (sd_mount()) {
		printf("sd mounted to /media/card\n");		
	} else {
		printf("sd mount : fail\n");		
	}

}
