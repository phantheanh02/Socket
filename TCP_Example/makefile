CC = gcc
CFLAGS = -Wall

all: TCP_server TCP_client

TCP_server: TCP_server.c
	$(CC) $(CFLAGS) -o TCP_server TCP_server.c

TCP_client: TCP_client.c
	$(CC) $(CFLAGS) -o TCP_client TCP_client.c
clean:
	rm -f TCP_server TCP_client
.PHONY: all clean
