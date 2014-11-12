#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include "app_05common.h"

#define SIZE_RCV_BUF 200

static unsigned char s_rxBuf[SIZE_ONE_PACKET + 10]; // 10: arbitrary but to extend the size

/*
 * Usage: [cmd] [serverIP]
 * e.g. ./cmd 127.0.0.1
 */


static bool rcvDataBlock(int destSocket, bool *prcvOK)
{
    int rcvdLen;
    int EOFpos;
    bool rcvdEOF = false;
    char dispBufStart[10 + 1];
    char dispBufEnd[10 + 1];
    int size;
    int loop;

    memset(s_rxBuf, 0, sizeof(s_rxBuf));
    rcvdLen = recv(destSocket, s_rxBuf, EOF_POS_B + 1, 0);
    if (rcvdLen == -1) {
        printf("rcvd fail\n");
        return; // fail
    }

    if (s_rxBuf[EOF_POS_A] == kDataCode || 
        s_rxBuf[EOF_POS_B] == kDataCode) {
        printf("rcvd Datacode at EOF_POS_A or EOF_POS_B %d\n", rcvdLen);
        printf("%c%c at 0,1\n", s_rxBuf[0], s_rxBuf[1]);
        *prcvOK = true; // HACKME: to avoid error
        return rcvdEOF; // HACKME: to avoid error
    }

    EOFpos = s_rxBuf[EOF_POS_A] * 256 + s_rxBuf[EOF_POS_B];

    if (EOFpos > 0) {
        printf("%d %d %d\n", rcvdLen, s_rxBuf[EOF_POS_A], s_rxBuf[EOF_POS_B]);
        rcvdEOF = true;
        s_rxBuf[EOFpos] = 0x00;
        size = EOFpos;
    } else {
        size = SIZE_ONE_PACKET;
    }    

    *prcvOK = true;
//    for (loop = 0; loop < size; loop++) {
    for (loop = 5; loop < size - 5; loop++) { // firsts and lasts are not kDataCode
        if (s_rxBuf[loop] != kDataCode) {
            *prcvOK = false;
        }
    }

#ifdef MODE_SILENT
    // do nothing
#else
    // disp only first 20 characters
    memset(dispBufStart, 0, sizeof(dispBufStart));
    memset(dispBufEnd, 0, sizeof(dispBufEnd));
    strncpy(dispBufStart, s_rxBuf, 10);
    strncpy(dispBufEnd, &s_rxBuf[EOF_POS_A - 10], 10);
    printf("rcvd packet [%s...%s] %d\n", dispBufStart, dispBufEnd, size);
//    printf("rcvd packet [%s] %d\n", s_rxBuf, size);
#endif


    return rcvdEOF;
}

static void dispCounter(int cnt)
{
#ifdef MODE_SILENT
    // do nothing
#else
    printf("%d: ", cnt++); // for debug
#endif                
}

int main(int argc, char **argv) {
    int loop;
    char destIP[80];
    unsigned short port = 9880;
    int destSocket;
    struct sockaddr_in destAddr;
    char toSendText[200];
    char rcvBuf[200];
    int rcvdLen;
    bool rcvdEOF;
    bool rcvOK;
    int cnt = 0;

    if (argc < 2) {
        printf("[cmd] [dest IP addr]\n");
        printf("e.g. ./a.out 127.0.0.1\n");
        return;
    }

    strcpy(destIP, argv[1]);
    printf("to %s\n", destIP);

    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_addr.s_addr = inet_addr(destIP);
    destAddr.sin_port = htons(port);
    destAddr.sin_family = AF_INET;

    destSocket = socket(AF_INET, SOCK_STREAM, 0);

    connect(destSocket, (struct sockaddr *) &destAddr, sizeof(destAddr));

    for(loop=0; loop<1; loop++) {
        sprintf(toSendText, "req");
        printf("tx:%s\n", toSendText);
        send(destSocket, toSendText, strlen(toSendText)+1, 0);

        rcvdLen = recv(destSocket, rcvBuf, SIZE_RCV_BUF, 0);

        if (rcvdLen != 0) {
            printf("rx:%s\n", rcvBuf);
//            usleep(100000);
            while(1) {
                dispCounter(cnt++);
                rcvdEOF = rcvDataBlock(destSocket, &rcvOK);
                if (rcvOK == false) {
                    printf("rcv failed\n");
                }

                usleep(100); // without this, Segmentation fault

                if (rcvdEOF) {
                    break;
                }
            }
        }
    }
    
    close(destSocket);
//    shutdown(destSocket, SHUT_WR);
}

