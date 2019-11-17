TARGET=cmtos

SOURCES=$(wildcard src/*.c) $(wildcard tests/*.c)
OBJECTS=$(SOURCES:.c=.o)

INC_DIRS=src tests
INC_PARAMS=$(foreach d, $(INC_DIRS), -I$d)

CC=gcc

GCCFLAGS= -coverage
CFLAGS  =$(GCCFLAGS) $(INC_PARAMS)
LDFLAGS =$(GCCFLAGS)

.PHONY: all clean test

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
	rm -f */*.o
	rm -f */*.gcda
	rm -f */*.gcno
	rm -f *.gcov
	rm -f *.info
	rm -rf report
