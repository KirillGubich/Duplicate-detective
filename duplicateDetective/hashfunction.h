#include <stdint.h>
#define DEFOULT_INIT 0xFFFFFFFF

uint32_t crc32(const unsigned char* buff, size_t len, uint32_t init_value);
unsigned char crc8(unsigned char* bytes, size_t len);