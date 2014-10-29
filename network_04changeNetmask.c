#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h> // for strncpy

int main(void)
{
	char *netMask = "255.0.0.0";
	int sockfd;
	struct ifreq ifr1;
	struct sockaddr_in *pin;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	pin = (struct sockaddr_in *)&ifr1.ifr_addr;

	pin->sin_family = AF_INET;

	inet_pton(AF_INET, netMask, &pin->sin_addr);

	strncpy(ifr1.ifr_name, "eth0", IFNAMSIZ - 1);

	if (ioctl(sockfd, SIOCSIFNETMASK, &ifr1) != 0) {
		perror("ioctl:");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("set Netmask to %s\n", netMask);

	close(sockfd);
	return 0;
}