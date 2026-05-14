CC=gcc
CFLAGS=-Wall -Werror -Wextra
LDLIBS = -lm
SOURCES=$(shell find . -name '*.c')
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(shell find . -name '*.h')
TARGET=sha256tool

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDLIBS) -o $@

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
