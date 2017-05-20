/* my client (reads from FIFOs) */

#include <sys/select.h> 
#include <unistd.h>  //read
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h> //constants O_RDWR, O_RDONLY, O_WRONLY
#include <sys/stat.h> // mkfifo

int main() {

	int fd1, fd2, fd3, max=0;
	fd_set rd_set;
	char buf[256];

	// open the FIFOs (created by other programs)
	// alternative 1
	fd1 = open("webserver", O_RDONLY, O_NONBLOCK); 
	fd2 = open("middleware", O_RDONLY, O_NONBLOCK); 
	fd3 = open("database", O_RDONLY, O_NONBLOCK); 

	// alternative 2: 
	/*
	fd1 = open("webserver", O_RDWR); 
	fd2 = open("middleware", O_RDWR); 
	fd3 = open("database", O_RDWR); 
	*/

	// find out maximum of file descriptors, because
	max=fd1>fd2?fd1:fd2;
	max=max>fd3?max:fd3;

	for(;;) {

		/// clear and fill set
		FD_ZERO(&rd_set);
		FD_SET(fd1, &rd_set);
		FD_SET(fd2, &rd_set);
		FD_SET(fd3, &rd_set);

		// use select with set to read
		// we want nothing to write, no exceptions, no timeout => NULL, NULL, NULL
		select(max+1, &rd_set, NULL, NULL, NULL);

		if(FD_ISSET(fd1, &rd_set)) {
			if(read(fd1, buf, sizeof(buf))>0)
				printf("[webserver]: %s\n", buf);
		}

		if(FD_ISSET(fd2, &rd_set)) {
			if(read(fd2, buf, sizeof(buf))>0)
				printf("[middlewareserver]: %s\n", buf);
		}

		if(FD_ISSET(fd3, &rd_set)) {
			if(read(fd3, buf, sizeof(buf))>0)
				printf("[databaseserver]: %s\n", buf);
		}

	}

}