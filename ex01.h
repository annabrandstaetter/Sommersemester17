#define _GNU_SOURCE /* for one of the headers */

#include <sys/shm.h>
#include <sys/stat.h> /* mkfifo */

#include <string.h> /* for memcpy */
#include <wait.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>  //read
#include <fcntl.h> //constants O_RDWR, O_RDONLY, O_WRONLY
#include <errno.h>

#define SHM_KEY 42

//#define _XOPEN_SOURCE 700
