CC = gcc

#Define model: open comment to switch other model
#CFLAGS = -DFORK
CFLAGS = -DMULTI_THREAD

ifeq ($(CFLAGS), -DFORK)
    LIBS = 
else ifeq ($(CFLAGS), -DMULTI_THREAD)
    LIBS = -pthread
endif

all: server client

server: server.c
	$(CC) $(CFLAGS) -o server server.c $(LIBS)

client: client.c
	$(CC) $(CFLAGS) -o client client.c
clean:
	rm -f server client
.PHONY: all clean
