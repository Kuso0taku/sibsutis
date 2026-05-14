#include <stdio.h>
#include <string.h>
#include "mysha256.h"

// convert hash to hex string
static void hash_to_hex(const uint8_t hash[32], char hex[65]) {
  for (size_t i=0; i<32; i++) sprintf(hex + i*2, "%02X", *(hash+i));
  *(hex+64) = '\0';
}

int main(int argc, char** argv) {
  

  return 0;
}
