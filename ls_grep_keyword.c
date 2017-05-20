/* exercise sheet05 task 2: ls|grep<keyword> with pipe, fork, dup2 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {

	int pfd[2], pid;
	if(pipe(pfd)==-1) perror("pipe");

	switch(pid=fork()) {

		case -1: perror("fork"); break;

		case 0:  /* child ls */
			if(close(pfd[0])==-1) perror("close read child");
			if(dup2(pfd[1], STDOUT_FILENO)==-1) perror("dup2 childs stdout to pipes write");
			if(close(pfd[1])==-1) perror("close write child"); 
			/* child writes now std to pipe, exec ls now */
			char* input[] = {"ls", NULL};
			if(execvp("ls", input)==-1) perror("execvp ls");
			// or: execvp("/bin/ls", input);
			// or: execv("/bin/ls", input);
			break;
		
		default: /* parent grep keyword */
			if(wait(0)==-1) perror("wait for child");
			if(close(pfd[1])==-1) perror("close write parent");
			if(dup2(pfd[0], STDIN_FILENO)==-1) perror("dup2 parents stdin to pipes read");
			if(close(pfd[0])==-1) perror("close read parent");
			/* parent ŕeads now std from pipe, exec grep keyword (keyword is argv[1]) now */
			if(execlp("grep", "grep", argv[1], (char *) NULL)==-1) perror("execl grep");
			// or: execlp("/bin/grep", "grep", argv[1], (char *) NULL);
			// or: execl("/bin/grep", "grep", argv[1], (char *) NULL);
			break;
		
	}

}


/* 

execlp = execvp, but one variadic function (-> list of arguments), one time array (-> vector), both have to end with (char *) NULL. First argument is the file to be executed, first argument of array/list also. Next arguments whatever program also wants (keyword for example). With "/" at beginning of file PATH is ignored and searched at this plavće

*/