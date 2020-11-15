#include <stdint.h>

#define BUFF 255
//#define makeStr(s) # s

struct fileArray
{
	struct fileInfo *list;
	int count;

};

struct fileInfo
{
	char name[BUFF];
	long size;
	uint32_t hash;
};

typedef struct fileArray FILES;

void findDup(char* path);

