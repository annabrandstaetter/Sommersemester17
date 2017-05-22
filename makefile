.PHONY: magic

CFLAGS= -std=c99

magic:
	gcc -o orakel orakel.c
	gcc -o ich ich.c
