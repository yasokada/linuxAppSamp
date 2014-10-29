#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h> // for strncpy
#include <stdlib.h> // for EXIT_FAILURE

int main(void)
{
	int sockfd;
	struct ifreq ifr1;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	strncpy(ifr1.ifr_name, "eth0", IFNAMSIZ - 1);
	if (ioctl(sockfd, SIOCSIFFLAGS, &ifr1) != 0) {
		perror("ioctl1:");
		exit(EXIT_FAILURE);
	}

	ifr1.ifr_flags &= ~(IFF_UP | IFF_RUNNING);

	if (ioctl(sockfd, SIOCSIFFLAGS, &ifr1) != 0) {
		perror("ioctl2:");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("set eth0 to down\n");
	close(sockfd);
	return 0;
}