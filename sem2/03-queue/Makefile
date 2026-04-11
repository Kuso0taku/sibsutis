CC=gcc
CFLAGS=-Wall -Werror -Wextra -lm
SOURCES=$(shell find -name '*.c')
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(shell find -name '*.h')
TARGET=program

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
