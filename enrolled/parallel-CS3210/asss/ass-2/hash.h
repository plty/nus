#include <stdint.h>

__device__ void sha256(uint8_t hash[32], const uint8_t * input, size_t len);
