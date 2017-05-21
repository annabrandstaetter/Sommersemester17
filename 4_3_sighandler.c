/* Task 3 */

#define _POSIX_SOURCE
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>

int pid, start, end; /* pid of child for sig handler */
bool unblock;

void c_sh(int signal) { /* deals with all three signals */

	if(signal==SIGALRM) /* unblocking signal SIGUSR2 when alarmed */
		unblock = true;
	else if(signal==SIGUSR1) 
		printf("second %d: child received SIGUSR1\n", (end=time(NULL))-start);
	else if(signal==SIGUSR2) { 
		printf("second %d: child received SIGUSR2\n", (end=time(NULL))-start); 
		exit(0);
	}

}

void child() {

	sigset_t set; /* blocking signal SIGUSR2 */
	sigemptyset(&set);		
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);

	unblock=false;

	struct sigaction sa; /* sigaction for SIGUSR1 and SIGUSR2 */
	sa.sa_handler=&c_sh;
	//sa.sa_restorer=NULL;
	sa.sa_flags=0;
	sigemptyset(&sa.sa_mask);
	
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGALRM, &sa, NULL);

	alarm(5);
	
	for(;;) { /* pause till interrupted by signal */
		pause();
		if(unblock==true) /* after received alarm: unblock SIGUSR2 again */
			sigprocmask(SIG_UNBLOCK, &set, NULL);
	}

}

void p_sh1(int signal) { kill(pid, SIGUSR1); }
void p_sh2(int signal) { kill(pid, SIGUSR2); }

void parent() {

	struct sigaction sa; /* sigaction struct for parent sig handler */
	sa.sa_flags=0;
	//sa.sa_restorer=NULL;
	sigemptyset(&sa.sa_mask);

	sa.sa_handler=&p_sh1; /* for each alarm send SIGUSR1*/
	sigaction(SIGALRM, &sa, NULL);
	
	for(int i=0; i<4; i++) {
		alarm(2);
		pause();
		if(i==2) {
			sa.sa_handler=&p_sh2; /* now SIGUSR2 when alarmed (needs reinstalling) */
			sigaction(SIGALRM, &sa,NULL);
		}
	}

	waitpid(pid, NULL, 0);

}

int main() {

	start = time(NULL);
	printf("second %d: starting program\n", (end=time(NULL))-start);
	pid=fork();
	switch(pid) {
		case -1: perror("fork"); break;
		case 0: child(); break;
		default: parent(); break;
	}

}

