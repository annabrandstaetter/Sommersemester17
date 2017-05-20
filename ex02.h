#define _GNU_SOURCE /* for one of the headers */

#include <sys/sem.h>
#include <sys/stat.h> /* mkfifo */
#include <sys/shm.h>

#include <wait.h>
#include <string.h> /* for memcpy */
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>  //read
#include <fcntl.h> //constants O_RDWR, O_RDONLY, O_WRONLY

#define SHM_KEY 123
#define SEM_KEY 456

union semun { /* semun struct for manipulating sem */

	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};