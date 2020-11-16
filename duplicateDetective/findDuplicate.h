#include <stdint.h>
#include <dirent.h>
#include "fileHash.h"

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

FILES* findDup(char* path);
int isDirExist(char* path);
void quickSortSize(struct fileInfo* s_arr, int first, int last);
void quickSortHash(struct fileInfo* s_arr, int first, int last);
FILES* getSemisizeFiles(FILES* files);
FILES* getDuplicates(FILES* files);
void insertFile(FILES* files, struct fileInfo file);

