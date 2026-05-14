#include <stdio.h>
#include <string.h>
#include "mysha256.h"

// convert hash to hex string
static void hash_to_hex(const uint8_t hash[32], char hex[65]) {
  for (size_t i=0; i<32; i++) sprintf(hex + i*2, "%02X", *(hash+i));
  *(hex+64) = '\0';
}

// give a usage/help to user 
static void usage(void) {
  printf("Usage:\n\t");
  printf("sha256tool <create|check> <path>\n");
}

// enum for modes
typedef enum {
  CREATE,
  CHECK
} Mode;

int main(int argc, char** argv) {
  if (argc != 3) {
    usage();
    return 1;
  }
  
  // define mode
  const char* mode = *(argv+1);
  Mode m;
  if (!strcmp(mode, "create")) m = CREATE;
  else if (!strcmp(mode, "check")) m = CHECK;
  else {
    usage();
    return 1;
  }
  
  // calc file hash
  const char* path = *(argv+2);
  uint8_t hash[32];
  if (sha256_file(path, hash)) {
    perror("Reading file error");
    return 1;
  }
  
  // convert hash to hex string
  char cur_hex[65];
  hash_to_hex(hash, cur_hex);

  if (m == CREATE) {
    char subpath[strlen(path) + 8]; // + ".sha256" + 1
    snprintf(subpath, sizeof(subpath), "%s.sha256", path);

    FILE* f = fopen(subpath, "w");
    if (!f) {
      perror("Can't create sha256 sum file");
      return 1;
    }
    
    fprintf(f, "%s %s\n", cur_hex, path);
    fclose(f);
    printf("Saved to %s\n", subpath);
  }

  else if (m == CHECK) {
    char subpath[strlen(path) + 8]; // + ".sha256" + 1
    snprintf(subpath, sizeof(subpath), "%s.sha256", path);

    FILE* f = fopen(subpath, "r");
    if (!f) {
      perror("Can't find sha256 sum file");
      return 1;
    }
    
    char saved_hex[65];
    size_t len = strlen(subpath)-7;

    if (fscanf(f, "%64s", saved_hex) == 1) {
      char ch = fgetc(f); // space
      if (ch != ' ') {
        fprintf(stderr, "Missing space after hash\n");
        fclose(f);
        return 1;
      }

      size_t i=0; // char index
      while ((ch = getc(f)) != '\n' && ch != EOF) {
        if (i > len || ch!=*(path+i)) {
          fprintf(stderr, "Invalid filename\n");
          fclose(f);
          return 1;
        }
        i++;
      }
      if (i != len) {
        fprintf(stderr, "Invalid filename length in sha256 sum file\n");
        fclose(f);
        return 1;
      }
    }
    else {
      fprintf(stderr, "Error sha256 sum file format\n");
      fclose(f);
      return 1;
    }
    fclose(f);
    
    if (!strcmp(cur_hex, saved_hex)) {
      printf("OK: file is fine\n");
    } else {
      printf("FAIL: hashes are different!\n");
      printf("Reference: %s\n", saved_hex);
      printf("Calculated: %s\n", cur_hex);
      return 1;
    }
  }
  else {
    usage();
    return 1;
  }

  return 0;
}
