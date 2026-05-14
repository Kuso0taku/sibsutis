#ifndef MYSHA256_H
#define MYSHA256_H

#include <stdint.h> // uint_t
#include <stddef.h> // size_t

typedef struct {
  uint8_t  block[64];  // current data block 
  size_t   datalen;    // how many bites already in block 
  uint64_t bitlen;     // message length
  uint32_t state[8];   // current hash (32 * 8 = 256 bits)
  uint32_t H[8];       // init values
  uint32_t K[64];      // round constants
} SHA256_CTX;

// context init (calculate H and K firt time)
void sha256_init(SHA256_CTX* ctx);

// give a part of data 
void sha256_update(SHA256_CTX* ctx, const uint8_t* data, size_t len);

// finish calculating and get 32 bytes (256 bits) of hash
void sha256_final(SHA256_CTX* ctx, uint8_t hash[32]);

// calculate full sha256 for file (0=ok, 1=error)
int sha256_file(const char* filename, uint8_t hash[32]);

#endif
