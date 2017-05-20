/* my database server (writes to FIFO) */

#include <unistd.h> // write, sleep
#include <time.h> // for srand(time(NULL)) and rand()
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h> // mkfifo

int main() {

	srand(time(NULL));
	mkfifo("database", 0666);
	int fd = open("database", O_WRONLY);
	//int fd = open("database", O_RDWR);
	int random = 0;

	for(;;) {
		random = rand()%6 + 2;
		sleep(random);
		write(fd, "databaseserver still there", sizeof("databaseserver still there"));
	}

}
