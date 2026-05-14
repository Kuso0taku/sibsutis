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

