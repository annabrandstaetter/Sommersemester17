.PHONY: all

FLAGS = -std=c99 -pthread -Werror -Wall
all:
	gcc $(FLAGS) -o ex21 ex02_1.c
	gcc $(FLAGS) -o ex22 ex02_2.c

clean:
	rm ex21 ex22