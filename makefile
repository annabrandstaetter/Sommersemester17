.PHONY: all

CFLAGS = -std=c99 -pthread -Werror -Wall
all:
	gcc $(CFLAGS) -o shmread fifo_shm_read.c -lrt
	gcc $(CFLAGS) -o shmwrite fifo_shm_write.c -lrt

clean:
	rm shmread shmwrite