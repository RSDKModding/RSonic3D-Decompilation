#ifndef READER_H
#define READER_H

#include "RetroEngine.hpp"

#if FORCE_CASE_INSENSITIVE
#include "fcaseopen.h"
#define FileIO                                          FILE
#define fOpen(path, mode)                               fcaseopen(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  fread(buffer, elementSize, elementCount, file)
#define fSeek(file, offset, whence)                     fseek(file, offset, whence)
#define fTell(file)                                     ftell(file)
#define fClose(file)                                    fclose(file)
#define fWrite(buffer, elementSize, elementCount, file) fwrite(buffer, elementSize, elementCount, file)
#elif RETRO_USING_SDL1 || RETRO_USING_SDL2 || RETRO_USING_SDL3
#define FileIO                                          SDL_RWops
#define fOpen(path, mode)                               SDL_RWFromFile(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  SDL_RWread(file, buffer, elementSize, elementCount)
#define fSeek(file, offset, whence)                     SDL_RWseek(file, offset, whence)
#define fTell(file)                                     SDL_RWtell(file)
#define fClose(file)                                    SDL_RWclose(file)
#define fWrite(buffer, elementSize, elementCount, file) SDL_RWwrite(file, buffer, elementSize, elementCount)
#else
#define FileIO                                          FILE
#define fOpen(path, mode)                               fopen(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  fread(buffer, elementSize, elementCount, file)
#define fSeek(file, offset, whence)                     fseek(file, offset, whence)
#define fTell(file)                                     ftell(file)
#define fClose(file)                                    fclose(file)
#define fWrite(buffer, elementSize, elementCount, file) fwrite(buffer, elementSize, elementCount, file)
#endif

#if !RETRO_USE_ORIGINAL_CODE && RETRO_USE_MOD_LOADER
#define READER_PATH_BUFFER(path)                                                                                                                     \
    char buffer[0x100];                                                                                                                              \
    StrCopy(buffer, path);                                                                                                                           \
    ModFilePath(buffer)
#else
#define READER_PATH_BUFFER(path) const char *buffer = path
#endif

struct FileInfo {
    byte *data;
    int size;
};

struct LevelDirectoryEntry {
    char levelName[4];
    char actNum[4];
    int levelNameLen;
    int actNumLen;
};

extern LevelDirectoryEntry *LDirectory;

void LoadFile(FileInfo *file, const char *path);
void LoadTexture(Texture **texturePtr, const char *path, bool useTexMips);

void LoadLevelModel(LMF *model, const char *path);
void SetLevelDirectory(const char *text, byte length, int index);
void SetActNumber(const char *text, byte length, int index);
void AllocateDirectories(int size);
void LoadDirectoryFile(FileInfo *file, int id, const char *fileName, int fileNameLen);
void LoadDirectoryActFile(FileInfo *file, int id, const char *fileName, int fileNameLen);
void LoadDirectoryGraphic(int id, const char *fileName, int fileNameLen);
void CreateDirectories();

void LoadModel(TMF *model, const char *path);
void LoadAnimationFile(Animator *animator, const char *path);
void LoadAnimationFile2(Animator *animator, const char *path);

#endif // !READER_H