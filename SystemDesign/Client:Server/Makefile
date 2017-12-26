all: client server
CC = gcc
CFLAGS = -ggdb

client: client_conc.c
	$(CC) $(CFLAGS) client_conc.c -o client
server: server_conc.c
	$(CC) $(CFLAGS) server_conc.c -o server

clean:
	rm -rf *.o client server

