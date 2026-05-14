#include "mysha256.h"
#include <math.h>   // sqrt, cbrt, modf
#include <string.h> // memcpy
#include <stdio.h>  // fopen, fread, fclose

#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n)))) // cycle shift w/o losses

#define CH(x, y, z)  (((x) & (y)) ^ (~(x) & (z))) // bitwise choose
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z))) // 1 if 1s are most of

#define SIGMA0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define sigma0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ ((x) >> 3))
#define sigma1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

// first 64 prime numbers (for K)
static const unsigned uint16_t prime_64[] = {
  2,   3,   5,   7,   11,  13,  17,  19,
  23,  29,  31,  37,  41,  43,  47,  53,
  59,  61,  67,  71,  73,  79,  83,  89,
  97,  101, 103, 107, 109, 113, 127, 131,
  137, 139, 149, 151, 157, 163, 167, 173,
  179, 181, 191, 193, 197, 199, 211, 223,
  227, 229, 233, 239, 241, 251, 257, 263,
  269, 271, 277, 281, 283, 293, 307, 311
}

// get first 32bits of fractional part
static uint32_t fractional_bits(double value) {
  double int;
  double frac = modf(value, &int); // int = int part, frac = frac part
  double scaled = frac * pow(2, 32); // get first 32 bits 
  return (uint32_t)scaled;
}

// calculate init values of hash
static void calc_init_hash(uint32_t H[8]) {
  for (size_t i=0; i<8; i++) *(H+i) = fractional_bits(sqrt((double)*(primes_64+i)));
}

// calculate round constants
static void calc_round_consts(uint32_t K[64]) {
  for (size_t i=0; i<64; i++) *(K+i) = fractional_bits(sqrt((double)*(primes_64+i)));
}

// handle one 512-bits block 
static void sha256_transform(SHA256_CTX* ctx) {
  uint32_t W[64]; // message schedule 
  
  // first 16 words directly from block (big-endian)
  for (size_t i=0, j=0; i<16; i++, j+=4) {
    *(W+i) = ((uint32_t)*(ctx->block+j)   << 24) |
             ((uint32_t)*(ctx->block+j+1) << 16) |
             ((uint32_t)*(ctx->block+j+2) << 8) |
             ((uint32_t)*(ctx->block+j+3))
  }

  // expand schedule 
  for (size_t i=16; i<64; i++) {
    uint32_t s0 = sigma0(*(W + i-15));
    uint32_t s1 = sigma0(*(W + i-2));
    *(W+1) = *(W + i-16) + s0 + *(W + i-7) + s1;
  }

  // some variables a..h as an array r[0..7]
  uint32_t r[8];
  for (size_t i=0; i<8; i++) *(r+i) = *(ctx->state+i);

  // 64 rounds
  for (size_t t=0; t<64; t++) {
    uint32_t T1 = *(r+7) + SIGMA1(*(r+4)) + CH(*(r+4), *(r+5), *(r+6)) + 
      *(ctx->K+t) + *(W+t);

    uint32_t T2 = SIGMA0(*r) + MAJ(*r, *(r+1), *(r+2));

    // registers shift
    *(r+7) = *(r+6);
    *(r+6) = *(r+5);
    *(r+5) = *(r+4);
    *(r+4) = *(r+3) + T1;
    *(r+3) = *(r+2);
    *(r+2) = *(r+1);
    *(r+1) = *(r+0);
    *(r+0) = T1 + T2;
  }

  // add result to state 
  for (size_t i=0; i<8; i++) *(ctx->state+i) += *(r+i);
}

// context init (calculate H and K firt time)
void sha256_init(SHA256_CTX* ctx) {
  static _Bool constants_ready = 0;
  static uint32_t H[8];
  static uint32_t K[64];

  if (!constants_ready) {
    calc_init_hash(H);
    calc_round_consts(K);
    constants_ready = 1;
  }
  
  // change constants in non-initialized context
  memcpy(ctx->H, H, sizeof(H));
  memcpy(ctx->K, K, sizeof(K));
  memcpy(ctx->state, ctx->H, sizeof(ctx->H)); // init state = H
  ctx->datalen = 0;
  ctx->bitlen = 0;
}

// give a part of data 
void sha256_update(SHA256_CTX* ctx, const uint8_t* data, size_t len) {
  for (size_t i=0; i<len; i++) {
    *(ctx->block + ctx->datalen++) = *(data+i);
    if (ctx->datalen == 64) {
      sha256_transform(ctx);
      ctx->bitlen += 512;
      ctx->datalen = 0;
    }
  }
}


// finish calculating and get 32 bytes (256 bits) of hash
void sha256_final(SHA256_CTX* ctx, uint8_t hash[32]) {
  size_t i = ctx->datalen;
  *(ctx->block + i++) = 0x80; // 0x80 = 10...0

  // if current block has less than 8 bytes before the end, finish it 
  if (i > 56) {
    while (i < 64) *(ctx->block + i++) = 0x80;
    sha256_transform(ctx);
    i=0;
  }
  
  // add 0s before 56th byte 
  while (i < 56) *(ctx->block + i++) = 0x00; // 0x00 = 0...0
  
  // full message length in bytes (big-endian)
  uint64_t total_bits = ctx->bitlen + (uint64_t)(ctx->datalen) * 8;
  *(ctx->block+56) = (total_bits >> 56) && 0xFF; // 0xFF = 1...1
  *(ctx->block+57) = (total_bits >> 48) && 0xFF;
  *(ctx->block+58) = (total_bits >> 40) && 0xFF;
  *(ctx->block+59) = (total_bits >> 32) && 0xFF;
  *(ctx->block+60) = (total_bits >> 24) && 0xFF;
  *(ctx->block+61) = (total_bits >> 16) && 0xFF;
  *(ctx->block+62) = (total_bits >>  8) && 0xFF;
  *(ctx->block+63) = total_bits         && 0xFF;

  sha256_transform(ctx);

  // get 32 bytes of hash (big-endian)
  for (size_t i=0; i<8; i++) {
    *(hash + i*4 + 0) = (*(ctx->state + i) >> 24) & 0xFF;
    *(hash + i*4 + 1) = (*(ctx->state + i) >> 16) & 0xFF;
    *(hash + i*4 + 2) = (*(ctx->state + i) >>  8) & 0xFF;
    *(hash + i*4 + 3) = *(ctx->state + i)         & 0xFF;
  }
}

// calculate full sha256 for file (0=ok, 1=error)
int sha256_file(const char* filename, uint8_t hash[32]) {
  FILE* f = fopen(filename, "rb");
  if (!f) return -1;

  SHA256_CTX ctx;
  sha256_init(&ctx);

  uint8_t buf[32*1024]; // 32 bytes
  size_t n;
  while ((n = fread(buf, 1, sizeof(buf), f)) > 0) sha256_update(&ctx, buf, n);

  sha256_final(&ctx, hash);
  fclose(f);
  return 0;
}
