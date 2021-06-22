TARGET = kilo kilo2

all: $(TARGET)

kilo: kilo.c
	$(CC) -o kilo kilo.c -Wall -W -pedantic -std=c99

kilo: kilo2.c
	$(CC) -o kilo2 kilo2.c -Wall -W -pedantic -std=c99

clean:
	rm -f *.o *.exe