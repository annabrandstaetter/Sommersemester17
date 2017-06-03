#include "exam.h"



int main() {

	mkfifo(FIFO, 0666);
	int fd;
	close(fd);

	if((fd = open(FIFO, O_WRONLY))<0) {
		perror("fifo error");
		exit(1);
	}

	int id=0;
	int q=2;
	int pid;
	
	sem_t* sem;
	if((sem = sem_open("myOtherSem", O_CREAT, 0666, 1)) == SEM_FAILED) { 
		perror("sem open error");
		return EXIT_FAILURE;
	} 

	for(int id=0; id<20; id++) {
		
		pid = fork();
	
		if(pid==-1) {
			perror("fork"); 
			exit(1);
		}

		else if(pid==0) {

			for(int i=0; i<100; i++) {
				sem_wait(sem); 
				write(fd, &id,  sizeof(int));	
				write(fd, &q, sizeof(int));
				sem_post(sem);
			}

			exit(0);

		}

		else
			continue;
		
		
	}

	if(pid>0) {
		while(wait(0)>0);
		if(sem_close(sem)<0) { perror("sem close error"); return EXIT_FAILURE; }
		if(close(fd)<0) { perror("fd close error"); return EXIT_FAILURE; }
		if(sem_destroy(sem)<0)  { perror("sem destroy error"); return EXIT_FAILURE; }
	

	}


	

	

}


















