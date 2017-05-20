#include "ex01.h"

// System V shared memory (shmget)

int main() {

	int shm_id=0, pid=0, fd=0;
	int* shm_ptr;

	if((shm_id = shmget(SHM_KEY, sizeof(int), IPC_CREAT|0666))<0) { /* get id of a new or already existing shm; */
		perror("shmget");
		return EXIT_FAILURE;
	}
	if((shm_ptr = shmat(shm_id, 0, 0))<0) { /* attach shm to process with shmid, address (or 0 if OS should choose) and flags */
		perror("shmat");
		return EXIT_FAILURE;
	}

	for(int i=0; i<100; i++) { /* create 100 child processes */
		pid=fork();
		if(pid==0)
			break;
	}

	if(pid<0) { perror("shmget"); return EXIT_FAILURE; } /* handling fork error */

	else if(pid==0) { /* children (already connected to shm) */
		for(int i=0; i<100; i++){
			(*shm_ptr)++; // increase value of shared memory per dereferencing the int*
		}
		exit(0);
	}

	else {

		while(wait(0) > 0); /* wait for all children */
		printf("value in shm after children: %d\n", *shm_ptr);

		if(mkfifo("RESULT_FIFO", 0666)<0 && errno != EEXIST) {
			perror("mkfifo");
			return EXIT_FAILURE;
		}
		if((fd = open("RESULT_FIFO", O_WRONLY))<0) { /* open fifo */
			perror("open fifo");
			return EXIT_FAILURE;
		}
		if(write(fd, shm_ptr, sizeof(*shm_ptr))<0) { /* write to fifo */
			perror("read from fifo");
			return EXIT_FAILURE;
		}

		close(fd);

	}

}