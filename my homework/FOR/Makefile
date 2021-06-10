CC := gcc
CFLAGS = -std=c99 -O0
TARGET = compiler1 compiler2

all: $(TARGET)

compiler1: lexer.c compiler1.c main.c
	$(CC) $(CFLAGS) $^ -o $@
compiler2: lexer.c compiler2.c main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o *.exe
