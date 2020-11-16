#include "findDuplicate.h"

FILES* findDup(char* path)
{
	// Checking directory existence
	if (!isDirExist(path)) {
		perror("diropen");
		exit(1);
	};
	
	FILES *files;
	files = (FILES*)malloc(sizeof(FILES));
	files->count = 0;
	files->list = (struct fileInfo*)malloc(sizeof(struct fileInfo));

	// Read files from directory
	readDirectory(files, path);

	// Sorting an array by file size
	quickSortSize(files->list, 0, files->count - 1);

	for (int i = 0; i < files->count; i++)
	{
		printf("%s size: %d \n", files->list[i].name, files->list[i].size);
	}
	printf("Total files: %d \n", files->count);
	puts("---------------------------------------");
	
	// Only leave files with duplicate sizes
	FILES *semisizeFiles;
	semisizeFiles = getSemisizeFiles(files);

	// Cleaning memory
	free(files->list);
	free(files);

	// Hashing files
	for (int i = 0; i < semisizeFiles->count; i++)
	{
		semisizeFiles->list[i].hash = fileHash(semisizeFiles->list[i].name);
	}

	quickSortHash(semisizeFiles->list, 0, semisizeFiles->count - 1);

	// find duplicates by hash
	FILES* duplicates;
	duplicates = getDuplicates(semisizeFiles);

	free(semisizeFiles->list);
	free(semisizeFiles);

	return duplicates;
}

// Get files with duplicate sizes
FILES* getSemisizeFiles(FILES* files)
{
	FILES *ssFiles;
	ssFiles = (FILES*) malloc(sizeof(FILES));

	ssFiles->count = 0;
	ssFiles->list = (struct fileInfo*)malloc(sizeof(struct fileInfo));

	int i = 0;

	while (i < files->count - 1)
	{
		if (files->list[i].size == files->list[i + 1].size)
		{
			int j = i;

			while (files->list[j].size == files->list[i].size)
			{
				insertFile(ssFiles, files->list[j]);
				j++;
			}

			i += j - i - 1;
		}

		i++;
	}

	return ssFiles;
}

// Get files with duplicate hash
FILES* getDuplicates(FILES* files)
{
	FILES* shFiles;
	shFiles = (FILES*)malloc(sizeof(FILES));

	shFiles->count = 0;
	shFiles->list = (struct fileInfo*)malloc(sizeof(struct fileInfo));

	int i = 0;

	while (i < files->count - 1)
	{
		if (files->list[i].hash == files->list[i + 1].hash)
		{
			int j = i;

			while (files->list[j].hash == files->list[i].hash)
			{
				insertFile(shFiles, files->list[j]);
				j++;
			}

			i += j - i - 1;
		}

		i++;
	}

	return shFiles;
}

// Checking directory existence
int isDirExist(char* path) 
{

	DIR* dir;
	dir = opendir(path);

	if (dir == NULL) 
		return 0;
	else
	{
		closedir(dir);
		return 1;
	}
		
}

// Sorting an array by size of file
void quickSortSize(struct fileInfo* s_arr, int first, int last)
{
	if (first < last)
	{
		int left, right, middle;
		left = first;
		right = last;
		middle = s_arr[(left + right) / 2].size;

		do
		{
			while (s_arr[left].size < middle) left++;
			while (s_arr[right].size > middle) right--;
			if (left <= right)
			{
				struct fileInfo tmp = s_arr[left];
				s_arr[left] = s_arr[right];
				s_arr[right] = tmp;
				left++;
				right--;
			}
		} while (left <= right);
		quickSortSize(s_arr, first, right);
		quickSortSize(s_arr, left, last);
	}
}

// Sorting an array by size of file
void quickSortHash(struct fileInfo* s_arr, int first, int last)
{
	if (first < last)
	{
		int left, right, middle;
		left = first;
		right = last;
		middle = s_arr[(left + right) / 2].hash;

		do
		{
			while (s_arr[left].hash < middle) left++;
			while (s_arr[right].hash > middle) right--;
			if (left <= right)
			{
				struct fileInfo tmp = s_arr[left];
				s_arr[left] = s_arr[right];
				s_arr[right] = tmp;
				left++;
				right--;
			}
		} while (left <= right);
		quickSortSize(s_arr, first, right);
		quickSortSize(s_arr, left, last);
	}
}

// Adding file to array
void insertFile(FILES* files, struct fileInfo file)
{
	files->count++;
	struct fileInfo* tmp;
	tmp = (struct fileInfo*)realloc(files->list, files->count * sizeof(struct fileInfo));

	if (tmp != NULL)
	{
		files->list = tmp;
		files->list[files->count - 1] = file;
	}
}