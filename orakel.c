#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h> 
#include <sys/stat.h> // mkfifo
#include <unistd.h>  //read
#include <fcntl.h> //constants O_RDWR, O_RDONLY, O_WRONLY
#include <unistd.h> // write, sleep
#include <time.h> // for srand(time(NULL)) and rand()
#include <string.h>
#include <stdbool.h>

int main() {

	srand(time(NULL));

	int fd_frage = open("frage", O_RDONLY);
	int fd_antwort = open("antwort", O_WRONLY);
	char buf_frage[256];
	char buf_antwort[256];
	int random;
	bool fragezeichen;

	for(;;) {

		fragezeichen=false;

		read(fd_frage, buf_frage, sizeof(buf_frage));
		
		random = (buf_frage[0]+buf_frage[1]+rand())%4;
		if(strstr(buf_frage, "?")==NULL)
			random = 42;

		switch(random) {
			case 0: strcpy(buf_antwort, "ja natürlich!"); break;
			case 1: strcpy(buf_antwort, "nein"); break;
			case 2: strcpy(buf_antwort, "weiß nicht, hab Angst"); break;
			case 3: strcpy(buf_antwort, "42"); break;
			default: strcpy(buf_antwort, "stell mir eine Frage ..."); break;
		}

		write(fd_antwort, buf_antwort, sizeof(buf_antwort));
	
	}

}