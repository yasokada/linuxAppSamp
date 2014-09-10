#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(int argc, char **argv)
{
    struct termios ttyio;
    struct termios stdio;
    struct termios old_stdio;
    int tty_fd;

    unsigned char c='D';
    tcgetattr(STDOUT_FILENO,&old_stdio);

    printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
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

    memset(&ttyio,0,sizeof(ttyio));
    ttyio.c_iflag=0;
    ttyio.c_oflag=0;
    ttyio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more informattyion
    ttyio.c_lflag=0;
    ttyio.c_cc[VMIN]=1;
    ttyio.c_cc[VTIME]=5;

    tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);      
    cfsetospeed(&ttyio,B115200);            // 115200 baud
    cfsetispeed(&ttyio,B115200);            // 115200 baud

    tcsetattr(tty_fd,TCSANOW,&ttyio);
    while (c!='q')
    {
        if (read(tty_fd,&c,1)>0) {
            write(STDOUT_FILENO,&c,1);  // on the serial port
        } 
        if (read(STDIN_FILENO,&c,1)>0) {
            write(tty_fd,&c,1);  // on the console
        }
    }

    close(tty_fd);
    tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);

    return 0;
}