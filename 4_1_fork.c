#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

	int x;
	for(int i=0; i<9; i++) {
		x=fork();
		if(x==0) {
			printf("child\n");
			exit(0);
		}
		wait(0);
	}

}
