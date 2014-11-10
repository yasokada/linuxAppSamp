#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SIZE_BUF 200

int main(void)
{
    unsigned short port = 9880;
    struct sockaddr_in rcvAddr;
    int sockfd, slen, rcvdLen;
    char rcvBuf[SIZE_BUF];
    int ret;

    slen = sizeof(rcvAddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) {
        exit(EXIT_FAILURE);
    }
    memset((char *) &rcvAddr, 0, sizeof(rcvAddr));     
    rcvAddr.sin_family = AF_INET;
    rcvAddr.sin_port = htons(port);
    rcvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd , (struct sockaddr*)&rcvAddr, sizeof(rcvAddr) );

    while(1) {
        fflush(stdout);
        rcvdLen = recvfrom(sockfd, rcvBuf, SIZE_BUF, 0, 
            (struct sockaddr *) &rcvAddr, &slen);
        if (rcvdLen == -1) {
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if (strstr(rcvBuf, "req") == NULL) {    // accept only "req"
            continue;
        }
        printf("rx: %s", rcvBuf);

        // TODO: remove <LF>

        ret = sendto(sockfd, rcvBuf, rcvdLen, 0, (struct sockaddr*) &rcvAddr, slen);         
        if (ret == -1) {
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }
    close(sockfd);
    return 0;
}
