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
    char dispBuf[20 + 1];
    int size;
    int loop;

    rcvdLen = recv(destSocket, s_rxBuf, EOF_POS_B + 1, 0);
    if (rcvdLen == -1) {
        printf("rcvd fail\n");
        return; // fail
    }

    EOFpos = s_rxBuf[EOF_POS_A] * 256 + s_rxBuf[EOF_POS_B];
    if (EOFpos > 0) {
        rcvdEOF = true;
        s_rxBuf[EOFpos] = 0x00;
        size = EOFpos;
    } else {
        size = SIZE_ONE_PACKET;
    }    

    *prcvOK = true;
    for (loop = 0; loop < size; loop++) {
        if (s_rxBuf[loop] != kDataCode) {
            *prcvOK = false;
        }
    }

#ifdef MODE_SILENT
    // do nothing
#else
    // disp only first 20 characters
    memset(dispBuf, 0, sizeof(dispBuf));
    strncpy(dispBuf, s_rxBuf, 20);
    printf("rcvd packet [%s...] %d\n", dispBuf, size);
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
            while(1) {
                dispCounter(cnt++);
                rcvdEOF = rcvDataBlock(destSocket, &rcvOK);
                if (rcvOK == false) {
                    printf("rcv failed\n");
                }

                usleep(10); // without this, Segmentation fault

                if (rcvdEOF) {
                    break;
                }
            }
        }
    }
    
    close(destSocket);
}

