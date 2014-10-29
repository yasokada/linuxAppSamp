#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>

/*
Lists names of network interfaces
*/

int main(void)
{
	struct ifconf config;
	struct ifreq *pifr;
	int sockfd;
	int numIf;
	int idx;
	int ret;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	// get ifc_len
	config.ifc_len = 0;
	config.ifc_ifcu.ifcu_buf = NULL;
	ioctl(sockfd, SIOCGIFCONF, &config);

	if (config.ifc_len == 0) {
		exit(EXIT_FAILURE);
	}

	// allocate buffer
	printf("ifc_len = %d\n", config.ifc_len);
	config.ifc_ifcu.ifcu_buf = (void *)malloc(config.ifc_len);

	// get names and etc
	ioctl(sockfd, SIOCGIFCONF, &config);

	numIf = config.ifc_len / sizeof(struct ifreq);
	pifr = (struct ifreq *)config.ifc_ifcu.ifcu_buf;

	for (idx=0; idx < numIf; idx++) {
		printf("name: %s\n", pifr[idx].ifr_name);
	}

	free(config.ifc_ifcu.ifcu_buf);  
	close(sockfd);

	return 0;
}
