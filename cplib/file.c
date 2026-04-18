/*
 * Hopefully I'll never have to touch this code again.
 * Interestingly, the current dir stuff works due to
 * a bunch of coincidences in the other functions.
 */


#include "common.h"
#include "file.h"

#ifdef TARGET_PC

#include <stdio.h>
#include <sys/stat.h> // mkdir
#include <stdlib.h>
#include <string.h>

#define BASE_PATH "./storage_emulated"
#define OS_SEPARATOR '/'

#else

#define BASE_PATH "\\fls0"
#define OS_SEPARATOR '\\'

#endif


typedef struct {
    // ReSharper disable once CppDeclaratorNeverUsed
    uint32_t unknown1;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint32_t fileSize;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint16_t creationDate;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint16_t creationTime;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint16_t lastModifiedDate;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint16_t lastModifiedTime;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint16_t unknown2;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint16_t lastAccessedDate;
} calcStat;

typedef struct {
    // ReSharper disable once CppDeclaratorNeverUsed
    uint8_t unknown0[4];
    // ReSharper disable once CppDeclaratorNeverUsed
    enum : uint16_t {
        calcEntryTypeFile = 0x1,
        calcEntryTypeDirectory = 0x5
    } calcEntryType;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint8_t unknown1[2];
    // ReSharper disable once CppDeclaratorNeverUsed
    uint32_t size;
    // ReSharper disable once CppDeclaratorNeverUsed
    uint8_t unknown2[8];
} calcFindInfo;

int CALC_FILE_close(int fd);
int CALC_FILE_findClose(int findHandle);
int CALC_FILE_findFirst(const wchar_t *path, int *findHandle, wchar_t *name, calcFindInfo *findInfoBuf);
int CALC_FILE_findNext(int findHandle, wchar_t *name, calcFindInfo *findInfoBuf);
int CALC_FILE_fstat(int fd, calcStat *buf);
int CALC_FILE_getAddr(int fd, int offset, const void **addr);
int CALC_FILE_lseek(int fd, int offset, int whence);
int CALC_FILE_mkdir(const char *path);
int CALC_FILE_open(const char *path, int flags);
int CALC_FILE_read(int fd, void *buf, int count);
int CALC_FILE_remove(const char *path);
int CALC_FILE_rename(const char *oldPath, const char *newPath);
int CALC_FILE_stat(const char *path, calcStat *buf);
int CALC_FILE_write(int fd, const void *buf, int count);


#define BUF_SIZE 128
static char currentDirectory[BUF_SIZE] = "/";


static void buildPath(char out[BUF_SIZE],
                      const char* base,
                      const char* cwd,
                      const char* name) {
    char temp[BUF_SIZE];
    strcpy(temp, base);

    if (strcmp(cwd, "/") == 0) {
        strcat(temp, "/");
        strcat(temp, name);
    } else {
        strcat(temp, cwd);
        strcat(temp, "/");
        strcat(temp, name);
    }

    strcpy(out, temp);
}

cpFile cpFileOpen(const char* path, const cpFileModes mode) {
    char fullPath[BUF_SIZE];
    buildPath(fullPath, BASE_PATH, currentDirectory, path);

#ifdef TARGET_PC

    char internalMode[BUF_SIZE];
    switch (mode) {
        case CP_FILE_MODE_WRITE:
            strcpy(internalMode, "wb");
            break;
        default: // read and any garbage data
            strcpy(internalMode, "rb");
            break;
    }

    FILE* internalFile = fopen(fullPath, internalMode);
    const bool isInvalid = !internalFile;

#else

    int internalMode;
    switch (mode) {
        case CP_FILE_MODE_WRITE:
            internalMode = (1 << 1) | (1 << 2);
            break;
        default: // read and any garbage data
            internalMode = 1 << 0;
            break;
    }

    int internalFile = CALC_FILE_open(fullPath, internalMode);
    const bool isInvalid = internalFile < 0;

#endif

    return (cpFile) {
        internalFile,
        mode,
        isInvalid
    };
}

int cpFileClose(const cpFile file) {
#ifdef TARGET_PC

    return fclose(file.internalFile);

#else

    return CALC_FILE_close(file.internalFile);

#endif
}

// return file to allow chaining of commands
cpFile cpFileSeek(const cpFile file, const int position, const int origin) {
#ifdef TARGET_PC

    fseek(file.internalFile, position, origin);

#else

    CALC_FILE_lseek(file.internalFile, position, origin);

#endif

    return file;
}

int cpFileRead(const cpFile file, const int byteCount, char* buf) {
    if (file.mode != CP_FILE_MODE_READ)
        return 1;

#ifdef TARGET_PC

    const int bytesRead = fread(buf, sizeof(char), byteCount, file.internalFile);
    return bytesRead < byteCount;

#else

    return CALC_FILE_read(file.internalFile, buf, byteCount) < 0;

#endif
}

int cpFileWrite(const cpFile file, const int byteCount, const char* buf) {
    if (file.mode != CP_FILE_MODE_WRITE)
        return 1;

#ifdef TARGET_PC

    const int writtenBytes = fwrite(buf, sizeof(char), byteCount, file.internalFile);
    return writtenBytes < byteCount;

#else

    return CALC_FILE_write(file.internalFile, buf, byteCount) < 0;

#endif
}

bool cpFileExists(const char* path) {
    char fullPath[BUF_SIZE];
    buildPath(fullPath, BASE_PATH, currentDirectory, path);

#ifdef TARGET_PC

    FILE* fptr = fopen(fullPath, "r");
    const bool ret = fptr;
    fclose(fptr);

    return ret;

#else

    const int file = CALC_FILE_open(fullPath, 1 << 0);
    const bool ret = file >= 0;
    CALC_FILE_close(file);

    return ret;

#endif
}

int cpFileCreate(const char* path) {
    char fullPath[BUF_SIZE];
    buildPath(fullPath, BASE_PATH, currentDirectory, path);

#ifdef TARGET_PC

    FILE* fptr = fopen(fullPath, "w");
    const int ret = !fptr;
    fclose(fptr);
    return ret;

#else

    const int file = CALC_FILE_open(fullPath, (1 << 1) | (1 << 2)); // write and create
    const bool ret = file < 0;
    CALC_FILE_close(file);

    return ret;

#endif
}

int cpFileRemove(const char* path) {
    char fullPath[BUF_SIZE];
    buildPath(fullPath, BASE_PATH, currentDirectory, path);

#ifdef TARGET_PC

    return remove(fullPath);

#else

    return !!CALC_FILE_remove(path);

#endif
}

int cpFileCopy(const char* srcPath, const char* destPath) {
    const cpFile srcFile = cpFileOpen(srcPath, CP_FILE_MODE_READ);

    if (srcFile.isInvalid)
        return 1;

    const int srcLength = cpGetFileLength(srcFile); // shouldn't fail

    char* srcBytes = malloc(sizeof(char) * srcLength);
    if (!srcBytes)
        return 1;

    if (cpFileRead(srcFile, srcLength, srcBytes))
        return 1;

    const cpFile destFile = cpFileOpen(destPath, CP_FILE_MODE_WRITE);

    if (destFile.isInvalid)
        return 1;

    if (cpFileWrite(destFile, srcLength, srcBytes))
        return 1;

    cpFileClose(srcFile);
    cpFileClose(destFile);
    free(srcBytes);
    return 0;
}

int cpFileMove(const char* srcPath, const char* destPath) {
    if (cpFileCopy(srcPath, destPath))
        return 1;

    return cpFileRemove(srcPath);
}


int cpGetFileLength(const cpFile file) {
    const int before = cpGetFilePosition(file);
    cpFileSeek(file, 0, CP_SEEK_END);
    const int length = cpGetFilePosition(file);
    cpFileSeek(file, before, CP_SEEK_START);

    return length;
}

int cpGetFilePosition(const cpFile file) {
#ifdef TARGET_PC

    return ftell(file.internalFile);

#else

    calcStat stat;
    CALC_FILE_fstat(file.internalFile, &stat);
    return (int)stat.fileSize;

#endif
}

char* cpGetFileExtension(const char* path) {
    const int pathLength = strlen(path);

    // find last separator
    int separatorIdx = 0;
    for (int i = pathLength-1; i >= 0; i--) {
        if (path[i] == '/') {
            separatorIdx = i;
            break;
        }
    }

    // copy only name
    const int nameLength = pathLength - separatorIdx - 1;
    char name[nameLength];
    strcpy(name, path + separatorIdx + 1);

    // find last .
    int dotIdx = -1;
    for (int i = nameLength-1; i >= 0; i--) {
        if (name[i] == '.') {
            dotIdx = i;
            break;
        }
    }

    if (dotIdx < 0) {
        // return empty string
        char* ret = malloc(sizeof(char));
        if (!ret) return ret;
        ret[0] = '\0';
        return ret;
    }

    char* extension = malloc(sizeof(char) * (nameLength - dotIdx));
    if (!extension) return extension;

    strcpy(extension, name + dotIdx + 1);
    return extension;
}

char* cpGetFileName(const char* path) {
    const int pathLength = strlen(path);

    // find last separator
    int separatorIdx = 0;
    for (int i = pathLength-1; i >= 0; i--) {
        if (path[i] == '/') {
            separatorIdx = i;
            break;
        }
    }

    // return only name
    char* name = malloc(sizeof(char) * (pathLength - separatorIdx));
    strcpy(name, path + separatorIdx + 1);
    return name;
}

char* cpGetFileNameWithoutExt(const char* path) {
    const int pathLength = strlen(path);

    // find last separator
    int separatorIdx = 0;
    for (int i = pathLength-1; i >= 0; i--) {
        if (path[i] == '/') {
            separatorIdx = i;
            break;
        }
    }

    // copy only name
    const int nameLength = pathLength - separatorIdx - 1;
    char name[nameLength];
    strcpy(name, path + separatorIdx + 1);

    // find last .
    int dotIdx = -1;
    for (int i = nameLength-1; i >= 0; i--) {
        if (name[i] == '.') {
            dotIdx = i;
            break;
        }
    }

    if (dotIdx < 0) {
        // return full name, no extension
        char* ret = malloc(sizeof(char) * (nameLength+1));
        if (!ret) return ret;
        strcpy(ret, name);
        return ret;
    }

    char* nameNoExt = malloc(sizeof(char) * (dotIdx+1));
    if (!nameNoExt) return nameNoExt;

    memcpy(nameNoExt, name, sizeof(char) * dotIdx);
    nameNoExt[dotIdx] = '\0';
    return nameNoExt;
}

char* cpGetDirectoryName(const char* path) {
    const int pathLength = strlen(path);

    int separatorIdx = 0;
    for (int i = pathLength-1; i >= 0; i--) {
        if (path[i] == '/') {
            separatorIdx = i;
            break;
        }
    }

    char* dirName = malloc(sizeof(char) * (separatorIdx+1));
    if (!dirName) return dirName;

    memcpy(dirName, path, separatorIdx);
    dirName[separatorIdx] = '\0';
    return dirName;
}

char* cpCurrentDirectory() {
    char* ret = malloc(sizeof(char) * (strlen(currentDirectory)+1));
    strcpy(ret, currentDirectory);
    return ret;
}

int cpMakeDirectory(const char* path) {
    char fullPath[BUF_SIZE];
    buildPath(fullPath, BASE_PATH, currentDirectory, path);

#ifdef TARGET_PC

    return mkdir(fullPath, 0777);

#else

    return CALC_FILE_mkdir(fullPath) < 0;

#endif
}

static void removeLastSegment(char path[BUF_SIZE]) {
    int length = strlen(path);

    if (length <= 1) { // root (e.g. "/")
        path[0] = '/';
        path[1] = '\0';
        return;
    }

    // remove trailing "/"
    if (path[length-1] == '/') {
        path[length-1] = '\n';
        length--;
    }

    // find last "/"
    int i;
    for (i = length-1; i >= 0; i--) {
        if (path[i] == '/')
            break;
    }

    if (i <= 0) {
        // go to root
        path[0] = '/';
        path[1] = '\0';
    } else {
        path[i] = '\0';
    }
}

static void appendSegment(char path[BUF_SIZE], char segment[BUF_SIZE]) {
    if (strcmp(path, "/") != 0) {
        strcat(path, "/");
    }
    strcat(path, segment);
}

int cpChangeDirectory(const char* path) {
    char newPath[BUF_SIZE];
    char token[BUF_SIZE];

    if (path[0] == '/')
        strcpy(newPath, "/");
    else
        strcpy(newPath, currentDirectory);

    int i = 0;
    int t = 0;

    while (true) {
        const char c = path[i];

        if (c == '/' || c == '\0') {
            token[t] = '\0';

            if (t > 0) {
                if (strcmp(token, ".") == 0) {
                    // do nothing
                } else if (strcmp(token, "..") == 0) {
                    removeLastSegment(newPath);
                } else {
                    appendSegment(newPath, token);
                }
            }

            t = 0;
            if (c == '\0')
                break;
        } else {
            token[t++] = c;
        }

        i++;
    }

    // root if empty
    if (strlen(newPath) == 0)
        strcpy(newPath, "/");

    strcpy(currentDirectory, newPath);
    return 0;
}
