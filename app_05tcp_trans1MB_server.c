#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> // for memset()
#include "app_05common.h"

#define SIZE_RCV 200

static void sendDataBlock(int destSocket, char *rxBuf)
{
    char txBuf[SIZE_ONE_PACKET + 10]; // 10: arbitrary but to extend the size

    strcpy(txBuf, rxBuf);
    strcat(txBuf, ",OK\n");

    send(destSocket, txBuf, strlen(txBuf)+1, 0);
    printf("rx: %s", txBuf);   
}

int main(void) {
    int ret;
    int rcvdLen;
    char rxBuf[SIZE_RCV];
    char txBuf[SIZE_ONE_PACKET + 10]; // 10: arbitrary but to extend the size
    unsigned short port = 9880;
    int srcSocket;
    int destSocket;
    struct sockaddr_in srcAddr;
    struct sockaddr_in dstAddr;
    int dstAddrSize = sizeof(dstAddr);

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

    while(1) {
        rcvdLen = recv(destSocket, rxBuf, SIZE_RCV, 0);
        if (rcvdLen == 0 || rcvdLen == -1) {
            ret = close(destSocket);
            break;
        }
        if (strstr(rxBuf, "req") == NULL) {
            continue;
        }

        // remove <LF>
        if (rxBuf[rcvdLen - 2] == '\n') {
            rxBuf[rcvdLen - 2] = 0x00;
        }

        sendDataBlock(destSocket, rxBuf);

        // TODO: send data

        // strcat(rxBuf, ",OK\n");

        // send(destSocket, rxBuf, strlen(rxBuf)+1, 0);

    }
}
