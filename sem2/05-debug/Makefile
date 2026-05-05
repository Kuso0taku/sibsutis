CC = gcc
CFLAGS = -Wall -Werror -Wextra -I$(CORE_DIR)
CFLAGS_DEBUG = -g -O0 -DDEBUG

CORE_DIR = /home/denchik/coding/prog/sem2/
LIBS = 01-struct 03-queue 05-debug
LIB_DIRS = $(addprefix $(CORE_DIR),$(LIBS))

SOURCES = $(shell find $(LIB_DIRS) -name '*.c' ! -name 'main.c' ! -name 'test.c')
OBJECTS = $(SOURCES:.c=.o)
HEADERS = $(shell find $(LIB_DIRS) -name '*.h')

APP = fioqueue

all: $(APP)

$(APP): $(OBJECTS)
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) $(OBJECTS) $(LDFLAGS) main.o -o $@

test: $(OBJECTS)
	$(CC) $(CFLAGS) -c test.c -o test.o
	$(CC) $(OBJECTS) $(LDFLAGS) test.o -o $@ 
	./test

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(APP)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APP) $(OBJECTS) main.o test.o

.PHONY: all clean
