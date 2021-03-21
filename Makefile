CC=gcc
CFLAGS=-Wall

hex: main.o best_move_and_win.o lists_and_print.o
	$(CC) -o hex main.o best_move_and_win.o lists_and_print.o

main.o: main.c
	$(CC) -c -o main.o main.c

best_move_and_win.o: best_move_and_win.c
	$(CC) -c -o best_move_and_win.o best_move_and_win.c

lists_and_print.o: lists_and_print.c
	$(CC) -c -o lists_and_print.o lists_and_print.c

.PHONY: clean

clean:
	rm -f hex main.o best_move_and_win.o lists_and_print.o