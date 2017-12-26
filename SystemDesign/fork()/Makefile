C=gcc
CFLAGS=-std=c99 -g -c -Wall
LDFLAGS=-std=c99 -g

# per project specific
SOURCES=Timer.c main.c
HEADERS=Timer.h
EXECUTABLE=prob3
ZIP_FILES=Makefile
ZIP_OUT=ai_hw4.zip
# end project specific

OBJECTS=$(SOURCES:.c=.o)
# start make targets
default: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS)  -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE) #$(ZIP_OUT)

zip: $(EXECUTABLE)
	zip $(ZIP_OUT) $(SOURCES) $(HEADERS) $(ZIP_FILES)

test: $(EXECUTABLE)
	./$(EXECUTABLE) Makefile my_ls.c
	./$(EXECUTABLE) $(HOME)/53/hw2

