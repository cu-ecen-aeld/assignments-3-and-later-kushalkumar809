clean:
	rm -f *.o writer *.elf *.map

all: writer.c
	gcc -g -Wall -c -o writer.o writer.c
	gcc -g -Wall -I/ writer.o -o writer

