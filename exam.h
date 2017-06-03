#define _GNU_SOURCE

#include <semaphore.h>	/* sem_open, sem_t, SEM_FAILED, sem_wait etc. */
#include <sys/mman.h>	/* for PROT_WRITE/PROT_READ, MAP_SHARED, munmap/mmap, shm_open/shm_unlink */
#include <sys/stat.h>	/* For mode constants */
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>	/* read, close, ftruncate */
#include <fcntl.h>	/* constants O_RDWR, O_RDONLY, O_WRONLY */

#include <sys/select.h> 
#include <unistd.h>  //read
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h> //constants O_RDWR, O_RDONLY, O_WRONLY
#include <sys/stat.h> // mkfifo

#define SHM "shm"
#define FIFO "FIFO_BANK"