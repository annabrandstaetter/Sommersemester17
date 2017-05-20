#include "fifo_shm.h"

// using POSIX standards (shm_open and sem_open)
// creates child processes, writes in fifo

int main() {

	int shm_fd=0, fd=0, pid=0; // fd for shm, fd for writing to fifo, pid for process id of parent and children
	int*shm_ptr=NULL;
	sem_t* sem;

	if((shm_fd = shm_open(SHM, O_RDWR, 0666)) < 0) { /* open shm (also inherited by children) */
		perror("shm open error");
		return EXIT_FAILURE;
	}
	if((shm_ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0)) < 0) { /* map shm to actual process */
		perror("shm mmap error");
		return EXIT_FAILURE;
	}
	if((sem = sem_open(SEM, O_CREAT, 0666, 1)) == SEM_FAILED) { /* open sem, value and mod ignored when already existing */
		perror("sem open error");
		return EXIT_FAILURE;
	}

	for(int i=0; i<100; i++) { /* create 100 child processes */
		pid=fork();
		if(pid==0)
			break;
	}

	if(pid<0) { perror("fork error"); exit(0); } /* check fork */

	else if(pid==0) { /* child */
		for(int i=0; i<100; i++){
			if(sem_wait(sem)<0) { perror("sem wait error"); return EXIT_FAILURE; } /* wait for sem */
			*shm_ptr=*shm_ptr+1; /* to do: increase value in shm */
			if(sem_post(sem)<0){ perror("sem post error"); return EXIT_FAILURE; } /* after critical area */
		}
		exit(0);
	}

	else { /* parent */
		while(wait(0) > 0); /* return of wait() is pid of terminated child, for error (no unwaited children left) -1*/

		if(sem_wait(sem)<0) { perror("sem wait error"); return EXIT_FAILURE; }
		printf("value in shm after children: %d\n", *shm_ptr);
		if(sem_post(sem)<0){ perror("sem post error"); return EXIT_FAILURE; }

		if(mkfifo("RESULT_FIFO", 0666)<0 && errno != EEXIST) {
			perror("mkfifo");
			return EXIT_FAILURE;
		}
		if((fd = open("RESULT_FIFO", O_WRONLY)) < 0) { /* open fifo */
			perror("fifo open error");
			return EXIT_FAILURE;
		}
		write(fd, shm_ptr, sizeof(*shm_ptr)); /* write to fifo */

		/* necessary? */
		if(sem_close(sem)<0) { perror("sem close error"); return EXIT_FAILURE; } 				/* close sem */
		if(munmap(shm_ptr, sizeof(int))<0) { perror("shm close error"); return EXIT_FAILURE; } 	/* unmap shm */
		//if(shm_unlink(SHM)<0) { perror("sem unlink error"); return EXIT_FAILURE; } 				/* unlink shm */
		if(close(shm_fd)<0) { perror("shm fd close error"); return EXIT_FAILURE; } 				/* close sh fd */
		if(close(fd)<0) { perror("fd close error"); return EXIT_FAILURE; }						/* close fd for reading */

	}

}