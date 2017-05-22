#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h> 
#include <sys/stat.h> // mkfifo
#include <unistd.h>  //read
#include <fcntl.h> //constants O_RDWR, O_RDONLY, O_WRONLY
#include <unistd.h> // write, sleep
#include <time.h> // for srand(time(NULL)) and rand()

int main() {

	mkfifo("frage", 0666);
	mkfifo("antwort", 0666);
	int fd_frage = open("frage", O_WRONLY);
	int fd_antwort = open("antwort", O_RDONLY);
	char buf_frage[256];
	char buf_antwort[256];

	for(;;) {

		printf("> ");
		scanf ("%[^\n]%*c", buf_frage);
		fflush(stdin);

		write(fd_frage, buf_frage, sizeof(buf_frage));
		read(fd_antwort, buf_antwort, sizeof(buf_antwort));
		
		printf("%s\n", buf_antwort);

	}

}