#ifndef CPLIB_FILE_H
#define CPLIB_FILE_H

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
    CP_FILE_MODE_READ = 0,
    CP_FILE_MODE_WRITE = 1,
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


cpFile cpFileOpen(const char* path, cpFileModes mode);
int cpFileClose(cpFile file);
cpFile cpFileSeek(cpFile file, int position, int origin); // why does stdlib use int???
int cpFileRead(cpFile file, int byteCount, char* buf);
int cpFileWrite(cpFile file, int byteCount, const char* buf);

bool cpFileExists(const char* path);
int cpFileCreate(const char* path);
int cpFileRemove(const char* path);
int cpFileCopy(const char* srcPath, const char* destPath);
int cpFileMove(const char* srcPath, const char* destPath);

int cpGetFileLength(cpFile file);
int cpGetFilePosition(cpFile file);
char* cpGetFileExtension(const char* path);
char* cpGetFileName(const char* path);
char* cpGetFileNameWithoutExt(const char* path);
char* cpGetDirectoryName(const char* path);

char* cpCurrentDirectory();
int cpMakeDirectory(const char* path);
int cpChangeDirectory(const char* path);

/*
 * Maybe:
 * - cpIsDirectory
 */

#endif