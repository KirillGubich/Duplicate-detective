// fileHash.c

#include "fileHash.h"
#include "hashfunction.h"

// File hashing using CRC-32
uint32_t fileHash(char file_name[255])
{
	uint8_t file_data[BUFFSIZE] = { '\0' };
	FILE* f;
	uint_least32_t crc = DEFOULT_INIT;
	errno_t err;
	err = fopen_s(&f, file_name, "rb");
	if (err == 0 && f != NULL) {
		while (!feof(f)) {
			int result = fread(file_data, 1, BUFFSIZE, f);
			if (result) {
				crc = crc32(file_data, result, crc);
			}
		}
		fclose(f);
	}

	// The value is inverted at the end so that it is possible 
	// to calculate the hash function in parts in a loop
	return crc ^ DEFOULT_INIT;
}