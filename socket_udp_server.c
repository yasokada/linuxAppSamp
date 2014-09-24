#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> // for memset()

#define SIZE_BUF 200

int main(void) {
    unsigned short port = 9880;
    int recvSocket;
    struct sockaddr_in rcvAddr;
    int ret;
    int rcvdLen;
    char szBuf[SIZE_BUF];

    memset(&rcvAddr, 0, sizeof(rcvAddr));
    rcvAddr.sin_port = htons(port);
    rcvAddr.sin_family = AF_INET;
    rcvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    recvSocket = socket(AF_INET, SOCK_DGRAM, 0);

    ret = bind(recvSocket, (struct sockaddr *) &rcvAddr, sizeof(rcvAddr));

    while(1) {
        rcvdLen = recvfrom(recvSocket, szBuf, SIZE_BUF, 0, NULL, NULL);
        if (rcvdLen == -1) {
            ret = close(recvSocket);
            break;
        }
        printf("rx: %s\n", szBuf);
    }
}

