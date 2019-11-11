TARGET=cmtos

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

CC=gcc

GCCFLAGS= -coverage
CFLAGS  =$(GCCFLAGS)
LDFLAGS =$(GCCFLAGS)

.PHONY: all clean

all: $(SOURCES) $(TARGET)

test: all
	./$(TARGET)
	gcov $(SOURCES)
	lcov --directory ./ --capture --output-file $(TARGET).info
	genhtml -o ./report/ $(TARGET).info

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.info
	rm -rf report
