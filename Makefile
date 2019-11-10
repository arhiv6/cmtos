TARGET=cmtos

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

CFLAGS=
LDFLAGS=

.PHONY: all clean

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f *.o
