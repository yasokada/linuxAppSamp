#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> // for memset()

#define SIZE_TXBUF 2048

static char txBuf[SIZE_TXBUF + 1];

int main(int argc, char **argv) {
    int loop;
    char destIP[80];
    unsigned short port = 9880;
    int destSocket;
    struct sockaddr_in destAddr;
    int idx;

    for(idx=0; idx<SIZE_TXBUF; idx++) {
        txBuf[idx] = 'C';
    }

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

    for(loop=0; loop<3; loop++) {
//        printf("tx:%s\n", txBuf);
        send(destSocket, txBuf, strlen(txBuf)+1, 0);
//        usleep(100000);
        usleep(1000);
    }
    
    close(destSocket);
}

