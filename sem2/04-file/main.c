#include "fioqueue.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// default queue size
#define QUEUE_SIZE 10000

// program mode 
typedef enum {
  TEXT, // 0
  BINARY // 1
} Mode;

// print help message
void usage(void) {
  printf("Usage: fioqueue [OPTIONS] <COMMAND> [ARGS] <FILE>\n\n");
  printf("Options:\n");
  printf("\t-h, --help\t\tPrint this message\n");
  printf("\t-t, --text\t\tUse text operations (default)\n");
  printf("\t-b, --binary\t\tUse binary operations\n\n");
  printf("Commands:\n");
  printf("\tsave\t\t\tSave queue\n");
  printf("\tload\t\t\tLoad queue\n");
  printf("\tget [LINE_INDEX]\tGet element by LINE_INDEX\n");
  printf("\tlist\t\t\tPrint all elements\n\n");
  printf("Examples:\n");
  printf("\tfioqueue --help\n");
  printf("\tfioqueue -b save queue.bin\n");
  printf("\tfioqueue --text load queue.txt\n");
  printf("\tfioqueue get 5 queue.txt\n");
  printf("\tfioqueue --binary list queue.bin\n");
}

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 5) {
    usage();
    return 1;
  }

  srand(time(NULL)); // make seed for random number

  Mode mode = TEXT; // default
  int arg_offset = 1; // command index

  if (**(argv+1) == '-') {
    if (!strcmp(*(argv+1), "-t") || !strcmp(*(argv+1), "--text")) {
      mode = TEXT;
    }
    else if (!strcmp(*(argv+1), "-b") || !strcmp(*(argv+1), "--binary")) {
      mode = BINARY;
    } else {
      fprintf(stderr, "Unknown option: %s\n\n", *(argv+1));
      usage();
      return 1;
    }
    arg_offset = 2;
  }
  
  const char* command = *(argv+arg_offset);

  if (!strcmp(command, "save")) {
    const char* filename = *(argv + arg_offset+1);
    
    printf("Queue creation...\n");

    Queue* q = rand_gen_matrix2d_in_queue(QUEUE_SIZE);
    if (!q) {
      perror("queue create");
      return 2;
    }

    printf("Queue created successful\n");
    
    int result = 0;
    
    switch (mode) {
      case TEXT: result = save_text_queue_to(q, filename); break;
      case BINARY: result = save_binary_queue_to(q, filename);
    }

    if (result) {
      fprintf(stderr, "save failed\n");
      queue_free(q);
      return 1;
    }

    //free_memory(q);
    printf("Saved %d elements to %s\n", QUEUE_SIZE, filename);
  }

/*
  else if (strcmp(command, "load")) {
  }

  else if (strcmp(command, "list")) {
  } 

  else if (!strcmp(command, "get")) {
  }

  else {

  }
*/
  return 0;
}
