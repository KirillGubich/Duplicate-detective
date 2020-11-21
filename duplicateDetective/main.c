#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "findDuplicate.h"

void outputFile(FILES* files);
void outputConsole(FILES* files);

int main()
{
	char path[255] = "D:\\Test";
	char* name;
	name = (char*)malloc(BUFF * sizeof(char));
	strcpy_s(name, BUFF, path);
	
	FILES* dup;
	dup = findDup(name);
	free(name);

	outputConsole(dup);

	getchar();
	return 0;
}

void outputConsole(FILES* files)
{
	int i = 0;

	while (i < files->count - 1)
	{
		if (files->list[i].hash == files->list[i + 1].hash)
		{
			int j = i;

			while (files->list[j].hash == files->list[i].hash)
			{
				printf("%s \n", files->list[j].name);
				j++;
			}

			puts("---------------------------------------------");
			i += j - i - 1;
		}

		i++;
	}

}

void outputFile(FILES* files)
{
	FILE* f;
	f = fopen("Duplicates.txt", "w");

	int i = 0;

	while (i < files->count - 1)
	{
		if (files->list[i].hash == files->list[i + 1].hash)
		{
			int j = i;

			while (files->list[j].hash == files->list[i].hash)
			{
				fputs(files->list[j].name, f);
				fputs("\n", f);
				j++;
			}

			fputs("---------------------------------------------", f);
			i += j - i - 1;
		}

		i++;
	}

	fclose(f);
}


