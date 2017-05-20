#include "ex02.h"

// System V shared memory and semaphore
// creates child processes, writes in fifo

int main() {

	int shm_id=0, pid=0, sem_id=0, fd=0;
	int* shm_ptr=NULL;

	if((shm_id = shmget(SHM_KEY, sizeof(int), 0666))<0) { /* get id of a new or already existing shm; */
		perror("shmget");
		return EXIT_FAILURE;
	}
	if((shm_ptr = shmat(shm_id, 0, 0))<0) { /* attach shm to process with shmid, address (or 0 if OS should choose) and flags */
		perror("shmat");
		return EXIT_FAILURE;
	}
	if((sem_id = semget(SEM_KEY, 1, 0666))<0) { /* here write access => alter sem allowed; open sem set with 1 sem and with unknown initial value */
		perror("semget");
		return EXIT_FAILURE;
	}
	
	struct sembuf sem_p[1]; /* struct array to implement p() operation */
	sem_p[0].sem_num = 0;
	sem_p[0].sem_op = -1; /* to decrease the value of sem */
	sem_p[0].sem_flg = 0;

	struct sembuf sem_v[1]; /* struct to implement v() operation */
	sem_v[0].sem_num = 0;
	sem_v[0].sem_op = +1; /* to increase the value of sem */
	sem_v[0].sem_flg = 0;
	
	for(int i=0; i<100; i++) {/* create 100 child processes */
		pid=fork();
		if(pid==0)
			break;
	}

	if(pid<0) { perror("shmget"); return EXIT_FAILURE; } /* handling fork error */

	else if(pid==0) { /* children (already connected to shm because inherited) */ 
		for(int i=0; i<100; i++){ 
			if(semop(sem_id, sem_p, 1)<0) { perror("semop p"); return EXIT_FAILURE; }
			*shm_ptr = *shm_ptr+1;
			if(semop(sem_id, sem_v, 1)<0) { perror("semop v"); return EXIT_FAILURE; }
		}
		exit(0);
	}

	else { /* parent */
		while(wait(0) > 0); /* wait for all children */

		if(semop(sem_id, sem_p, 1)<0) { perror("semop p"); return EXIT_FAILURE; }
		printf("value in shm after children: %d\n", *shm_ptr);
		if(semop(sem_id, sem_v, 1)<0) { perror("semop v"); return EXIT_FAILURE; }

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

		if(semctl(sem_id, 0, IPC_RMID)<0) { /* rm sem, ignores 0 (semnum) */
			perror("IPC_RMID");
			return EXIT_FAILURE;
		}

	}

}