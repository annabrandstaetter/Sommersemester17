#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
// Linux scheduling: round robin with priorities
/* After fork 2 processe, kernel can decide, which gets CPU time. Between parent and child: always the parent? */
int main() {

	int x;
	for(int i=0; i<16; i++) {
		x=fork();
		if(x==0)
			break;
	}
	switch(x) {	
		case -1: perror("failed\n");
		case 0: printf("pid: %d\n", getpid()); exit(0);
		default: printf("16 child processes have been created.\n");
	}

}
