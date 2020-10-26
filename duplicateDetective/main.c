#include <stdio.h>
#include "hashfunction.h"
#include "fileHash.h"

int main()
{

	uint32_t res;
	res = fileHash("D:\\test.txt");

	printf("hash: %u ", res);

	getchar();
	return 0;
}

