.PHONY: all

FLAGS = -Wall -Werror -std=c99
all:
	gcc $(FLAGS) -o data database.c
	gcc $(FLAGS) -o middle middleware.c
	gcc $(FLAGS) -o web webserver.c
	gcc $(FLAGS) -o service service.c

clean:
	rm data middle web service

rmpipes:
	rm middleware webserver database
