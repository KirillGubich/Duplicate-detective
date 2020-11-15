#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "hashfunction.h"
#include "fileHash.h"
#include "findDuplicate.h"
#include <dirent.h>



int main()
{

	//uint32_t res;
	//res = fileHash("E:\\Epic Games\\GTAV\\x64q.rpf");

	//printf("hash: %u ", res);
	char path[255] = "D:\\Test";
	char* name;
	name = (char*)malloc(BUFF * sizeof(char));
	strcpy_s(name, BUFF, path);
	


	findDup(name);

	getchar();
	return 0;
}


