
main : main.c snake.o
	gcc -o main main.c snake.o -lncurses

snake.o : snake.c snake.h
	gcc -o snake.o snake.c -O2 -c 
