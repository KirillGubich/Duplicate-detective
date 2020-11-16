#define _CRT_SECURE_NO_WARNINGS
#include <dirent.h>
#include <string.h>
#include "findDuplicate.h"

#define BUFF 255
#define FOLDER 16384

void readDirectory(FILES* files, char* path);
void addFile(FILES* files, char* name);