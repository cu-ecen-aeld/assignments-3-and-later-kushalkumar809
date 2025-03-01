src = aesdsocket.c
cc = $(CROSS_COMPILE)gcc

all : $(TARGET)
	$(CC) -g -Wall -c $(SRC) -o aesdsocket.o
	$(CC) -g -Wall aesdsocket.o -o aesdsocket

clean :
	rm -rf aesdsocket.o aesdsocket
