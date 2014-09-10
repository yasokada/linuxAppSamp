#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(int argc, char **argv)
{
	struct termios tio;
	struct termios stdio;
	struct termios preSetting;
	int tty_fd;
    char szBuf[20];

	unsigned char c='D';
	tcgetattr(STDOUT_FILENO, &preSetting);

	if (argc < 1) {
    	printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
    	return;
	}
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);      
    cfsetospeed(&tio,B115200);            // 115200 baud
    cfsetispeed(&tio,B115200);            // 115200 baud

    tcsetattr(tty_fd,TCSANOW,&tio);
    while (c!='q')
    {
	    if (read(tty_fd,&c,1)>0) {
	        write(STDOUT_FILENO,&c,1);              // on serial port
            strcpy(szBuf, "_tty");
            write(STDOUT_FILENO, szBuf, strlen(szBuf));
	    }
	    if (read(STDIN_FILENO,&c,1)>0) {            
            write(tty_fd,&c,1);                     // on the console
            strcpy(szBuf, "_std");
            write(STDOUT_FILENO, szBuf, strlen(szBuf));
        }
    }

    close(tty_fd);
    tcsetattr(STDOUT_FILENO,TCSANOW,&preSetting);
 
    return 0;
}