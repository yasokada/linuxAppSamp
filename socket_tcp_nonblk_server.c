#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> // for memset()
#include <sys/ioctl.h> // for FIONBIO

#define MAX_SIZE 50000
#define SIZE_RCV 32768
static char szBuf[MAX_SIZE];

int main(void) {
    int ret;
    int rcvdLen;
    unsigned short port = 9880;
    int srcSocket;
    int destSocket;
    struct sockaddr_in srcAddr;
    struct sockaddr_in dstAddr;
    int dstAddrSize = sizeof(dstAddr);
    int val = 1; // for nonblocking comm

    memset(&srcAddr, 0, sizeof(srcAddr));
    srcAddr.sin_port = htons(port);
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    srcSocket = socket(AF_INET, SOCK_STREAM, 0);

    bind(srcSocket, (struct sockaddr *) &srcAddr, sizeof(srcAddr));

    listen(srcSocket, 1);

    printf("before accept\n");
    destSocket = accept(srcSocket, (struct sockaddr *) &dstAddr, &dstAddrSize);
    printf("after accept %s\n", inet_ntoa(dstAddr.sin_addr));

    ioctl(destSocket, FIONBIO, &val); // for nonblocking comm

    while(1) {
        memset(szBuf, 0, sizeof(szBuf));

        rcvdLen = recv(destSocket, szBuf, SIZE_RCV+1, 0);
        if (rcvdLen == -1) { // waiting
            printf(".");
            usleep(100);
            continue;
        }
//        if (rcvdLen == 0 || rcvdLen == -1) {
        if (rcvdLen == 0) { // shutdown by peer
            ret = close(destSocket);
            break;
        }
//        printf("rx: [%d] %s\n", strlen(szBuf), szBuf);
//        printf("rx: %d \n", strlen(szBuf));
    }
}
