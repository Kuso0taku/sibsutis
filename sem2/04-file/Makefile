CC = gcc
CFLAGS = -Wall -Werror -Wextra -I$(CORE_DIR)

CORE_DIR = /home/denchik/coding/prog/sem2/
LIBS = 01-struct 03-queue 04-file
LIB_DIRS = $(addprefix $(CORE_DIR),$(LIBS))

SOURCES = $(shell find $(LIB_DIRS) -name '*.c' ! -name 'main.c') main.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = $(shell find $(LIB_DIRS) -name '*.h')

APP = fioqueue

all: $(APP)

$(APP): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APP) $(OBJECTS)

.PHONY: all clean
