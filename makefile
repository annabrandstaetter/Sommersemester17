.PHONY: all

FLAGS = -std=c99 -pthread
all:
	gcc $(FLAGS) -o client exam_ps_bs.c -lrt
	gcc $(FLAGS) -o server exam_ps_bs_server.c -lrt