#include <stdio.h>
#include <sys/reboot.h>

int main(void)
{
	sleep(3);

	reboot(RB_AUTOBOOT);
    // does not correctly work on QEMU
}