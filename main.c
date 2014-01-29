#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define MODEM "/dev/ttyUSB0"
#define BAUDRATE B38400


int main(void) {
    struct termios tio;
    unsigned char c='D';
    int tty_fd;
    int fd, i;

    char data[2048];
    int count = 0;

    if ((fd = open("dump.bin", O_RDONLY))) {

    	while (read(fd, &c, 1) > 0) {
    		data[count++] = c;
    	}

    	close (fd);
    } else {
        printf("dump.bin not found\n");
        return -1;
    }

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;
    if((tty_fd = open(MODEM , O_RDWR)) == -1) {
        printf("Error while opening\n");
        return -1;
    }

    cfsetospeed(&tio,BAUDRATE);
    cfsetispeed(&tio,BAUDRATE);
    tcsetattr(tty_fd,TCSANOW,&tio);

    while (1) {
    	while (c!='\r'){
    		if (read(tty_fd,&c,1)>0){
    			printf("%c", c);
    		}
    	}

    	for (i = 0; i < count; ++i) {
    		write(tty_fd, &(data[i]), 1);
    	}

    	c = '\r';
    	write(tty_fd, &c, 1);

    	c = '\0';
    }

    close(tty_fd);

	return 0;
}


