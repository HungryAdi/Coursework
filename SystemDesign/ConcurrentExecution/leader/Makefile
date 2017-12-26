PORT = 7286
PROGRAMS = server client
S = -std=c99 -ggdb

both: $(PROGRAMS)

server: server.c
	gcc $S server.c -lpthread -o server

client: client.c Timer.c Timer.h
	gcc $S client.c Timer.c -lpthread -o client

test: both
	./server $(PORT) &
	echo Starting client
	./client nikolai-jakov $(PORT)
	ls -lr Thread_*
	du
clean:
	/bin/rm -rf $(PROGRAMS) Thread_*

