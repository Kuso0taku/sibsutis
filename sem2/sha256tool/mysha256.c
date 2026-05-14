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
static void calc_init_hash(uint32_t K[64]) {
  for (size_t i=0; i<64; i++) *(K+i) = fractional_bits(sqrt((double)*(primes_64+i)));
}
