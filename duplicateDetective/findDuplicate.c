#include "findDuplicate.h"
#include <dirent.h>

void findDup(char* path)
{
	
	DIR* dir;
	dir = opendir(path);

	if (!dir) {
		perror("diropen");
		exit(1);
	};

	closedir(dir);
	
	FILES *files;
	files = (FILES*)malloc(sizeof(FILES));
	files->count = 0;
	files->list = (struct fileInfo*)malloc(sizeof(struct fileInfo));

	readDirectory(files, path);

	for (int i = 0; i < files->count; i++)
	{
		printf("%s size: %d\n", files->list[i].name, files->list[i].size);
	}

}

