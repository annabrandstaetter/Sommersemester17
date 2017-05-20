#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

	int x;
		
	switch(x=fork()) {	
		case -1: perror("fork"); exit(1);
		case 0: printf("%d\n", getpid()); exit(0);
		default: waitpid(x, 0, 0);
	}

}