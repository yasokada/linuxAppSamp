#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> // for memset()
#include <stdbool.h>
#include "app_05common.h"

#define SIZE_RCV_BUF 200 // for short length texts

static unsigned char s_largeData[SIZE_LARGE_DATA];

static unsigned char s_txBuf[SIZE_ONE_PACKET + 10]; // 10: arbitrary but to extend the size

static void sendReqOK(int destSocket, char *rxBuf, int rcvdLen)
{
    memset(s_txBuf, 0, sizeof(s_txBuf));
    strcpy(s_txBuf, rxBuf);

    // remove <LF>
    if (s_txBuf[rcvdLen - 2] == '\n') {
        s_txBuf[rcvdLen - 2] = 0x00;
    }

    strcat(s_txBuf, ",OK");

    send(destSocket, s_txBuf, strlen(s_txBuf)+1, 0);
    printf("tx: %s\n", s_txBuf);   
}

static void setLargeData(void)
{
    int idx;

    for(idx=0; idx<SIZE_LARGE_DATA; idx++) {
        s_largeData[idx] = 'C';
    }
}

static void sendOnePacket(int destSocket, char *pData, int start, int size, bool sendEOF)
{
    unsigned char posA, posB;
    char dispBuf[20 + 1];

    if (size < SIZE_ONE_PACKET || sendEOF) {
        posA = size / 256;
        posB = size % 256; // TODO: check +1 is needed or not
    }

    memset(s_txBuf, 0, sizeof(s_txBuf));
    memcpy(s_txBuf, &pData[start], size);

    s_txBuf[EOF_POS_A] = posA;
    s_txBuf[EOF_POS_B] = posB;

    send(destSocket, s_txBuf, EOF_POS_B + 1, 0);

    // disp only first 20 characters
    memset(dispBuf, 0, sizeof(dispBuf));
    strncpy(dispBuf, s_txBuf, 20);
    printf("send packet [%s...] %d\n", dispBuf, size);
}

static void sendDataBlock(int destSocket)
{
//    strcpy(s_largeData, "Hello WorldABCDEF"); 
        // "Hellow World" is data
        // "ABCEDF" is not data
    int loop;

    int leftSize = SIZE_LARGE_DATA;

    // for (loop = 0; loop < 100; loop++) {
    //     printf("%d :", loop);
    //     sendOnePacket(destSocket, s_largeData, /* start= */0, SIZE_ONE_PACKET);
    // }
    // sendOnePacket(destSocket, s_largeData, /* start= */0, 11);

    while(1) {
        if (leftSize == 0) {
            break;
        }
        if (leftSize > SIZE_ONE_PACKET) {
            sendOnePacket(destSocket, s_largeData, /* start= */0, SIZE_ONE_PACKET, false);
        } else {
            sendOnePacket(destSocket, s_largeData, /* start= */0, leftSize, true);
            break;
        }
        leftSize -= SIZE_ONE_PACKET;
    }

}

int main(void) {
    int ret;
    int rcvdLen;
    char rxBuf[SIZE_RCV_BUF];
    char txBuf[SIZE_ONE_PACKET + 10]; // 10: arbitrary but to extend the size
    unsigned short port = 9880;
    int srcSocket;
    int destSocket;
    struct sockaddr_in srcAddr;
    struct sockaddr_in dstAddr;
    int dstAddrSize = sizeof(dstAddr);

    setLargeData();

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
        rcvdLen = recv(destSocket, rxBuf, SIZE_RCV_BUF, 0);
        if (rcvdLen == 0 || rcvdLen == -1) {
            ret = close(destSocket);
            break;
        }
        if (strstr(rxBuf, "req") == NULL) {
            continue;
        }

        sendReqOK(destSocket, rxBuf, rcvdLen);
        usleep(100000);

        sendDataBlock(destSocket);
    }
}
