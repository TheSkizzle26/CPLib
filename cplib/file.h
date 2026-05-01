#ifndef CPLIB_FILE_H
#define CPLIB_FILE_H

#ifdef CPLIB_ENABLE_FILE

#ifdef TARGET_PC

#include <stdio.h>

// use the already existing macros
#define CP_SEEK_START SEEK_SET
#define CP_SEEK_CURRENT SEEK_CUR
#define CP_SEEK_END SEEK_END

#else

#define CP_SEEK_START 0
#define CP_SEEK_CURRENT 1
#define CP_SEEK_END 2

#endif


typedef enum {
    CP_FILE_MODE_READ       = 0,
    CP_FILE_MODE_WRITE      = 1,
} cpFileModes;

typedef struct {
#ifdef TARGET_PC
    FILE* internalFile;
#else
    int internalFile;
#endif

    cpFileModes mode;
    bool isInvalid; // can be used to check if the file's opened correctly
} cpFile;

typedef struct {
    int count;
    char** names;
} cpListDirResult;


cpFile cpFileOpen(const char* path, cpFileModes mode); // Open a file.
int cpFileClose(cpFile file); // Close an open file.
cpFile cpFileSeek(cpFile file, int position, int origin); // Move to a specific position inside a file.
int cpFileRead(cpFile file, int byteCount, char* buf); // Read a specific amount of bytes from a file.
int cpFileWrite(cpFile file, int byteCount, const char* buf); // Write some bytes to a file.

bool cpFileExists(const char* path); // Check if a file exists.
int cpFileCreate(const char* path); // Create a file.
int cpFileRemove(const char* path); // Remove a file.
int cpFileCopy(const char* srcPath, const char* destPath); // Copy a file to a new path.
int cpFileMove(const char* srcPath, const char* destPath); // Move a file to a new path.

int cpGetFileLength(cpFile file); // Get a file's length in bytes.
char* cpGetFileExtension(const char* path) [[unsequenced]]; // Get a file's extension, for example ".txt".
char* cpGetFileName(const char* path) [[unsequenced]]; // Get a file's name, for example "file.txt".
char* cpGetFileNameWithoutExt(const char* path) [[unsequenced]]; // Get a file's name without the extension, for example "file".
char* cpGetDirectoryPath(const char* path) [[unsequenced]]; // Get the specified path without the file name.

char* cpCurrentDirectory(); // Get the current working directory.
int cpMakeDirectory(const char* path); // Create a directory.
int cpChangeDirectory(const char* path); // Change the current working directory.
bool cpIsDirectory(const char* path); // Check if a path is a directory;
bool cpIsFile(const char* path); // Check if a path is a file.
void cpListDirectory(const char* path, cpListDirResult* result); // List the contents of a directory. Call cpFreeListDirResults() once done with the results.
void cpFreeListDirResult(cpListDirResult* result) [[unsequenced]]; // Free the results of cpListDirectory().

#endif

#endif