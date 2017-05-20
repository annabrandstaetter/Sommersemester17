/* my middlewareserver (writes to FIFO) */

#include <unistd.h> // write, sleep
#include <time.h> // for srand(time(NULL)) and rand()
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h> // mkfifo

int main() {

	srand(time(NULL));
	mkfifo("middleware", 0666);
	int fd = open("middleware", O_WRONLY);
	//int fd = open("middleware", O_RDWR);
	int random = 0;

	for(;;) {
		random = rand()%6 + 2;
		sleep(random);
		write(fd, "middlewareserver still there", sizeof("middlewareserver still there"));	
	}

}
