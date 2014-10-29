#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

/*
Displays list of interfaces and interface types
When IPv4, displays also [IP address] and [netmask]
*/

static void *toSinAddr(struct sockaddr *p)
{
	return &( ((struct sockaddr_in *)p)->sin_addr );
}

int main(void)
{
	struct ifaddrs *phead;
	struct ifaddrs *pitr;
	int ret;
	char caddr[256], cnetmask[256];
	char cfamily[100];

	ret = getifaddrs(&phead);
	if (ret != 0) {
		exit(EXIT_FAILURE);
	}

	pitr = phead;
	while(1) {
		if (pitr == NULL) {
			break;
		}
		memset(caddr, 0, sizeof(caddr));
		memset(cnetmask, 0, sizeof(cnetmask));
		switch(pitr->ifa_addr->sa_family) {
			case AF_INET:
				strcpy(cfamily, "IPv4");
				inet_ntop(AF_INET, toSinAddr(pitr->ifa_addr), caddr, sizeof(caddr));
				inet_ntop(AF_INET, toSinAddr(pitr->ifa_netmask), cnetmask, sizeof(cnetmask));
				break;
			case AF_INET6:
				strcpy(cfamily, "IPv6");
				break;
			case AF_PACKET:
				strcpy(cfamily, "AF_PACKET");
				break;			
			default:
				strcpy(cfamily, "other");
		}
		printf("%s: \t%s", pitr->ifa_name, cfamily);
		printf("\t%s\t%s", caddr, cnetmask);
		printf("\n");
		pitr = pitr->ifa_next;
	}

	freeifaddrs(phead);

	return 0;
}

