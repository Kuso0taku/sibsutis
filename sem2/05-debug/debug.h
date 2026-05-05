#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG

#include <stdio.h>

  #define LOG(fmt, ...) fprintf(stderr, "[LOG] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#else 
  #define LOG(fmt, ...) ((void)0)

#endif

#endif
