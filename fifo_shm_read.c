#include "fifo_shm.h"

// using POSIX standards (shm_open and sem_open)
// initializes shm and sem, reads from fifo

int main() {
	
	int shm_fd=0, fd=0, buf=0; // fd for shm, fd for reding from fifo, buf for storing value of fifo
	int* shm_ptr=NULL;
	sem_t* sem;

	if((shm_fd = shm_open(SHM, O_CREAT|O_RDWR, 0666)) < 0) { /* open shm */
		perror("shm open error");
		return EXIT_FAILURE;
	}
	if(ftruncate(shm_fd, sizeof(int))<0) { /* change size of shm to size of int; otherwise: Bus error (core dumped) */
		perror("shm open error"); 
		return EXIT_FAILURE; 
	}
	if((shm_ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0)) < 0) { /* maps shm to actual process */
		perror("shm mmap error");
		return EXIT_FAILURE;
	}

	if((sem = sem_open(SEM, O_CREAT, 0666, 1)) == SEM_FAILED) { /* open sem with initial value 1 */
		perror("sem open error");
		return EXIT_FAILURE;
	}

	if(sem_wait(sem)<0) { perror("sem wait error"); return EXIT_FAILURE; } /* p() operation: block till sem >0, then sem decremented (-1) */
	*shm_ptr = 0; /* write 0 in shm */
	if(sem_post(sem)<0){ perror("sem post error"); return EXIT_FAILURE; } /* v() operation: increments sem (+1) */

	if(mkfifo("RESULT_FIFO", 0666)<0 && errno != EEXIST) {
		perror("mkfifo");
		return EXIT_FAILURE;
	}

	if((fd = open("RESULT_FIFO", O_RDONLY)) < 0) { /* open fifo */
		perror("fifo open error");
		return EXIT_FAILURE;
	}
	read(fd, &buf, sizeof(buf)); /* block till sth. to read, then read from fifo */
	printf("received value of FIFO: %d\n", buf); 

	/* necessary? */
	if(sem_close(sem)<0) { perror("sem close error"); return EXIT_FAILURE; } 				/* close sem */
	if(munmap(shm_ptr, sizeof(int))<0) { perror("shm close error"); return EXIT_FAILURE; } 	/* unmap shm */
	if(shm_unlink(SHM)<0) { perror("sem unlink error"); return EXIT_FAILURE; } 				/* unlink shm */
	if(close(shm_fd)<0) { perror("shm fd close error"); return EXIT_FAILURE; } 				/* close sh fd */
	if(close(fd)<0) { perror("fd close error"); return EXIT_FAILURE; }						/* close fd for reading */

}

