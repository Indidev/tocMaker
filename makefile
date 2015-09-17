SRC	=tocMaker.c
CC	=/usr/bin/gcc
CFLAGS	=-Wall -std=c99

tocMaker: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o tocMaker
