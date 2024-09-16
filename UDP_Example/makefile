CC = gcc
CFLAGS = -Wall

all: UDP_server UDP_client

UDP_server: UDP_server.c
	$(CC) $(CFLAGS) -o UDP_server UDP_server.c

UDP_client: UDP_client.c
	$(CC) $(CFLAGS) -o UDP_client UDP_client.c
clean:
	rm -f UDP_server UDP_client
.PHONY: all clean
