OBJS = test
CC = gcc -I../../lib
L = ../../lib

all: $(OBJS)

test: ../test.c board.c $(L)/stdio.c $(L)/string.c
	$(CC) $^ -o $@

clean:
	rm *.exe $(OBJS)
