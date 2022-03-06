// readDirectiry.c

#include "readdirectory.h"

// Read files from directory
void readDirectory(FILES *files, char *path)
{
	DIR* dir;
	dir = opendir(path);

	struct dirent* fInfo;

	// Read while there are files
	while ((fInfo = readdir(dir)) != NULL)
	{
		// Skip "." and ".."
		if (strcmp(fInfo->d_name, ".") != 0 && strcmp(fInfo->d_name, "..") != 0)
		{
			// Full path to file
			char fullPath[BUFF];
			strcpy_s(fullPath, BUFF, path);
			strncat_s(fullPath, BUFF, "\\", 3);
			strncat_s(fullPath, BUFF, fInfo->d_name, BUFF);
			
			// Add file into array, if it is not folder
			if (fInfo->d_type != FOLDER)
			{
				addFile(files, fullPath);
			}
			else
			{
				// Read files from folder
				readDirectory(files, fullPath);
			}			
		}			
	}

	closedir(dir);
}

// Size of file
long fileSize(char* path)
{
	HANDLE hfile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// If file has incorrect name - NULL
	if (hfile == NULL)
	{
		return NULL;
	}

	DWORD file_size = GetFileSize(hfile, NULL);
	CloseHandle(hfile);
	return file_size;
}

// Adding file to array
void addFile(FILES* files, char* name)
{

	// If file has incorrect size - return
	if (fileSize(name) == NULL)
	{
		return;
	}

	files->count++;
	struct fileInfo* tmp;
	tmp = (struct fileInfo*)realloc(files->list, files->count * sizeof(struct fileInfo));

	if (tmp != NULL)
	{
		files->list = tmp;	
		strcpy(files->list[files->count - 1].name, name);
		files->list[files->count - 1].size = fileSize(name);
		files->list[files->count - 1].hash = 0;
	}
}