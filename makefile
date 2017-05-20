.PHONY: all

FLAGS = -std=c99 -Werror -Wall
all:
	gcc $(FLAGS) -o ex11 ex01_1.c
	gcc $(FLAGS) -o ex12 ex01_2.c

clean:
	rm ex11 ex12