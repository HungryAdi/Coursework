CC = gcc
CFLAGS = -std=c99 -g -c
LFLAGS = -std=c99 -g
EXE = my_ls
ZIP = ai_hw3.zip

default: $(EXE)

$(EXE): main.o my_ls.o
	$(CC) $(LFLAGS) my_ls.o main.o -o $(EXE)

main.o: my_ls.o  main.c
	$(CC) $(CFLAGS) main.c 

my_ls.o: my_ls.c 
	$(CC) $(CFLAGS) my_ls.c	

clean:
	rm my_ls.o main.o $(EXE)

#zip: $(EXE) hw3.script
	#zip $(ZIP) my_ls.c myls.h main.c hw3.script Makefile
