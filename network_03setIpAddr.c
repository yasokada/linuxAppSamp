#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h> // for strncpy

/*
Set IP address. (require root priviledge)

!!!ATTENTION!!!
Before executing this, check the following IP is O.K. to set
in the network, or execute this in an isolated network.
*/

int main(void)
{
	char *ipAddr = "192.168.0.180";
	int sockfd;
	struct ifreq ifr1;
	struct sockaddr_in *pin;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	pin = (struct sockaddr_in *)&ifr1.ifr_addr;
	pin->sin_family = AF_INET;

	inet_pton(AF_INET, ipAddr, &pin->sin_addr);

	strncpy(ifr1.ifr_name, "eth0", IFNAMSIZ - 1);

	if (ioctl(sockfd, SIOCSIFADDR, &ifr1) != 0) {
		perror("ioctl");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("set to %s\n", ipAddr);

	close(sockfd);

	return 0;
}
