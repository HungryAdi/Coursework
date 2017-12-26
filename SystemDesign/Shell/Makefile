C=gcc
CFLAGS=-std=c99 -g -c
LDFLAGS=-std=c99 -g

# per project specific
SOURCES=my_shell.c
HEADERS=
EXECUTABLE=my_shell
ZIP_FILES=hw3.script Makefile
ZIP_OUT=ai_hw3.zip
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

