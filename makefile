CFLAGS	=-Wall -g -std=c99

tocMaker: tocMaker.c

all: tocMaker

clean:
	$(RM) tocMaker
