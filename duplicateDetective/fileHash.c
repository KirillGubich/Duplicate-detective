#include "fileHash.h"
#include "hashfunction.h"

// File hashing using CRC-32
uint32_t fileHash(char file_name[255])
{
	uint32_t hash = DEFOULT_INIT;
	uint8_t file_data[BUFFSIZE] = { '\0' };
	size_t n_bytes_read;

	FILE* file;
	file = fopen(file_name, "r");

	// The file is hashed in blocks until it is read in full
	while (n_bytes_read = fread(file_data, 1, BUFFSIZE, file))
	{
		hash = crc32(file_data, n_bytes_read, hash);
	}

	fclose(file);

	return hash ^ 0xFFFFFFFFUL;
}