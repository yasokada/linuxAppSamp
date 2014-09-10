#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(int argc, char **argv)
{
    struct termios tty_io;
    struct termios preSet;
    int tty_fd;

    unsigned char c=0;
    tcgetattr(STDOUT_FILENO, &preSet);

    if (argc < 2) {
        printf("[cmd] [device of serial tty]\n");
        printf("e.g. %s /dev/ttyPS0\n", argv[0]);
        return;
    }

    memset(&tty_io, 0, sizeof(tty_io));
    tty_io.c_iflag=0;
    tty_io.c_oflag=0;
    tty_io.c_cflag=CS8 | CREAD | CLOCAL;           // 8n1
    tty_io.c_lflag=0;
    tty_io.c_cc[VMIN]=1;
    tty_io.c_cc[VTIME]=5;

    tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);      
    cfsetospeed(&tty_io,B115200);            // 115200 baud
    cfsetispeed(&tty_io,B115200);            // 115200 baud

    tcsetattr(tty_fd, TCSANOW, &tty_io);
    while (c!='q')
    {
        if (read(tty_fd, &c, 1)>0) {
            write(STDOUT_FILENO, &c, 1);
        } 
    }

    close(tty_fd);
    tcsetattr(STDOUT_FILENO, TCSANOW, &preSet);

    return 0;
}