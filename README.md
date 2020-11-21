# Duplicate-detective
A software tool that allows to find duplicate files in a given directory.

The duplicate search algorithm consists of several basic steps:
1) Read all files from a directory.
2) Find and keep only duplicate size files.
3) Calculate the hash for these files.
4) Determine duplicates by hash.

<dirent.h> was used to read the directory. 
It is not present by default at Microsoft Visual Studio 2019 and was taken from: https://github.com/tronkko/dirent
