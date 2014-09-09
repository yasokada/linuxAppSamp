#include <stdio.h>
#include <stdbool.h> // for bool
#include <sys/mount.h> // for mount(2)

static bool sd_mount(void)
{
	int ret;
	system("mkdir /media/card"); // no check for return value

	ret = mount("/dev/mmcblk0p1", "/media/card", "vfat",  MS_MGC_VAL | MS_SYNCHRONOUS, NULL);
//	ret = mount("/dev/sdf1", "/media/card", "vfat", MS_MGC_VAL | MS_SYNCHRONOUS, NULL);
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
