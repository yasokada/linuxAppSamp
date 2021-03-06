#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> // for memset()

#define SIZE_BUF 200

int main(int argc, char **argv) {
    int loop;
    char destIP[80];
    unsigned short port = 9880;
    int destSocket;
    struct sockaddr_in destAddr;
    char toSendText[80];
    int rcvdLen;
    char szBuf[200];

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

    destSocket = socket(AF_INET, SOCK_DGRAM, 0);

    for(loop=0; loop<3; loop++) {
        sprintf(toSendText, "req\n");
        printf("tx:%s", toSendText);
        sendto( destSocket, toSendText, strlen(toSendText)+1, 0, 
            (struct sockaddr *)&destAddr, sizeof(destAddr) );
        rcvdLen = recvfrom(destSocket, szBuf, SIZE_BUF, 0, NULL, NULL);
        if (rcvdLen != -1) {
            // remove <LF>
            if (szBuf[rcvdLen - 2] == '\n') {
                szBuf[rcvdLen - 2] = 0x00;
            }
            printf("rx:%s\n", szBuf);
        }
        printf("---\n");
        sleep(1);
    }

    close(destSocket);
}
