#include "ex01.h"

// System V shared memory (shmget)

int main() {
	
	int shm_id=0, value=0, buf=0, fd=0;
	int* shm_ptr=NULL;

	if((shm_id = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666))<0) { /* get id of a new or already existing shm; */
		perror("shmget");
		return EXIT_FAILURE;
	}
	if((shm_ptr = shmat(shm_id, 0, 0))<0) { /* attach shm to process with shmid, address (or 0 if OS should choose) and flags */
		perror("shmat");
		return EXIT_FAILURE;
	}

	memcpy(shm_ptr, &value, sizeof(value)); /* write integer in shm */

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
	close(fd);

}