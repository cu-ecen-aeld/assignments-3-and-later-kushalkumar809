src = aesdsocket.c
cc = $(CROSS_COMPILE)gcc

all : $(target)
	$(CC) -g -Wall -c $(src) -o aesdsocket.o
	$(CC) -g -Wall aesdsocket.o -o aesdsocket

clean :
	rm -rf aesdsocket.o aesdsocket
