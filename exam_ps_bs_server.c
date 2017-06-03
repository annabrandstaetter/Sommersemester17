#include "exam.h"


int main() {

	int fd;
	int buf[2];
	buf[0]=0;
	buf[1]=0;
	int pid;
	int *shm_ptr;
	int shm_fd;
	int signs;

	close(fd);
	fd = open(FIFO, O_RDONLY);

	if((shm_fd = shm_open(SHM, O_CREAT|O_RDWR, 0666)) < 0) { 
		perror("shm open error");
		return EXIT_FAILURE;
	}
	if(ftruncate(shm_fd, 20*sizeof(int))<0) { 
		perror("shm open error"); 
		return EXIT_FAILURE; 
	}
	if((shm_ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0)) < 0) { 
		perror("shm mmap error");
		return EXIT_FAILURE;
	}

	sem_t* sem;
	
	if((sem = sem_open("mySem", O_CREAT, 0666, 1)) == SEM_FAILED) { 
		perror("sem open error");
		return EXIT_FAILURE;
	}


	while((signs=read(fd, buf, 2*sizeof(int)))>0) {

		pid = fork();

		if(pid==-1) {
			perror("fork"); 
			exit(1);
		}


		else if(pid==0) {				
			
			sem_wait(sem);
			*(shm_ptr+buf[0]) += buf[1];
			sem_post(sem);
			exit(0);

		}

		else
			continue;


	}

	if(pid>0) {
		while(wait(0)>0);

		for(int i=0; i<20; i++) {
			printf("%d\n", *(shm_ptr+i));
		}

		if(sem_close(sem)<0) { perror("sem close error"); return EXIT_FAILURE; }
		if(munmap(shm_ptr, sizeof(int))<0) { perror("shm close error"); return EXIT_FAILURE; }
		if(shm_unlink(SHM)<0) { perror("sem unlink error"); return EXIT_FAILURE; }
		if(close(shm_fd)<0) { perror("shm fd close error"); return EXIT_FAILURE; }
		if(close(fd)<0) { perror("fd close error"); return EXIT_FAILURE; }
		if(sem_destroy(sem)<0)  { perror("sem destroy error"); return EXIT_FAILURE; }

	}
	


}
	