#include "ex02.h"

// System V shared memory and semaphore (shmget and semget)
// initializes shm and sem, reads from fifo

int main() {
	
	int shm_id=0, sem_id=0, value=0, buf=0, fd=0;
	int* shm_ptr=NULL;

	if((shm_id = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666))<0) { /* get id of a new or already existing shm; */
		perror("shmget");
		return EXIT_FAILURE;
	}
	if((shm_ptr = shmat(shm_id, 0, 0))<0) { /* attach shm to process with shmid, address (or 0 if OS should choose) and flags */
		perror("shmat");
		return EXIT_FAILURE;
	}
	if((sem_id = semget(SEM_KEY, 1, IPC_CREAT|0666))<0) { /* here write access => alter sem allowed; open sem set with 1 sem and with unknown initial value */
		perror("semget");
		return EXIT_FAILURE;
	}

	union semun sem_un; /* sem union (copy structure to header!) needed to set value to 1 => to use sem safe */
	sem_un.val = 1;
	semctl(sem_id, 0, SETVAL, sem_un); /* SETVAL sets value of sem with sem_id to val of sem_un */

	struct sembuf sem_p[1]; /* struct array to implement p() operation */
	sem_p[0].sem_num = 0;
	sem_p[0].sem_op = -1; /* to decrease the value of sem */
	sem_p[0].sem_flg = 0;

	struct sembuf sem_v[1]; /* struct to implement v() operation */
	sem_v[0].sem_num = 0;
	sem_v[0].sem_op = +1; /* to increase the value of sem */
	sem_v[0].sem_flg = 0;

	if(semop(sem_id, sem_p, 1)<0) { perror("semop p"); return EXIT_FAILURE; } /* each (1) element of the sem_p array is applied to the sem_num th (0) sem of sem_id */
	memcpy(shm_ptr, &value, sizeof(value));
	if(semop(sem_id, sem_v, 1)<0) { perror("semop v"); return EXIT_FAILURE; } /* same as sem_p, but with the struct (= the operation) sem_v */

	if(mkfifo("RESULT_FIFO", 0666)<0 && errno != EEXIST) {
		perror("mkfifo");
		return EXIT_FAILURE;
	}
	if((fd = open("RESULT_FIFO", O_RDONLY))<0) { /* open fifo */
		perror("open fifo");
		return EXIT_FAILURE;
	}
	if(read(fd, &buf, sizeof(buf))<0) { /* read from fifo, blocks till sth to read */
		perror("read from fifo");
		return EXIT_FAILURE;
	}
	printf("received value of FIFO: %d\n", buf);

	if(semctl(sem_id, 0, IPC_RMID)<0) { /* rm sem, ignores 0 (semnum) */
		perror("IPC_RM");
		return EXIT_FAILURE;
	}

}

