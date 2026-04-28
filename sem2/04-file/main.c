#include "fioqueue.h"
#include <stdio.h>
#include <string.h>

typedef enum {
  BINARY,
  TEXT
} Mode;

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
  printf("\tfioqueue -b save queue.bin\n");
  printf("\tfioqueue --text load queue.txt\n");
  printf("\tfioqueue get 5 queue.txt\n");
  printf("\tfioqueue --binary list queue.bin\n");
}

//int main(int argc, char* argv[]) {
int main(void) {
  usage();
  return 0;
}
