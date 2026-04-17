#ifndef CPLIB_FILE_H
#define CPLIB_FILE_H

#include "common.h"

#ifdef TARGET_PC
#include <stdio.h>
#define CP_ROOT_PATH "./emulated"
#define CP_PATH_SEPERATOR = "/"
#else
#define CP_ROOT_PATH "\\\\fls0"
#define CP_PATH_SEPERATOR = "\\"
#endif


typedef enum {
    CP_FILE_MODE_READ = 0,
    CP_FILE_MODE_WRITE = 0,
} cpFileModes;

typedef struct {
#ifdef TARGET_PC
    FILE* internalFile;
#else
    int internalFile;
#endif

    cpFileModes mode;
} cpFile;


cpFile cpFileOpen(char* path, cpFileModes mode);
int cpFileClose(cpFile file);
cpFile cpFileSeek(cpFile file, size_t position);
int cpFileRead(cpFile file, size_t byteCount, char* buf);
int cpFileWrite(cpFile file, char* buf);

bool cpFileExists(char* path);
int cpFileCreate(char* path);
int cpFileRemove(char* path);
int cpFileCopy(char* path);
int cpFileMove(char* path);

size_t cpGetFileLength(cpFile file);
char* cpGetFileExtension(char* path);
char* cpGetFileName(char* path);
char* cpGetFileNameWithoutExt(char* path);
char* cpGetDirectoryName(char* path);

int cpMakeDirectory(char* path);
int cpChangeDirectory(char* path);


#endif