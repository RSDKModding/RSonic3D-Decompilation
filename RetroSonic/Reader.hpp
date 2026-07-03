#ifndef READER_H
#define READER_H

#include "RetroEngine.hpp"

#if defined (FORCE_CASE_INSENSITIVE) && defined (ijtorhiomjbtn)
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

struct FileInfo {
    byte *data;
    int size;
};

struct LevelDirectoryEntry {
    char levelName[4];
    char actNum[4];
    uint levelNameLen;
    uint actNumLen;
};

struct LMFMesh {
    LVertex *vertices;
    float *colors;
    ushort numVertices;
    ushort *indices;
    ushort numIndices;
};

struct LMF {
    byte surfaceCount;
    byte unused;
    byte surfaceID[10];
    ushort columns;
    ushort rows;
    float startX;
    float startZ;
    LMFMesh ***tiles;
};

struct TMF {
    Vertex *vertices;
    ushort numVertices;
    ushort *indices;
    ushort numIndices;
};

struct AnimationState {
    byte frameCount;
    byte unknown1;
    ushort array_2[128];
    byte loopIndex;
    byte frameDuration;
};

struct AnimationNode {
    ushort *vertexIDs;
    ushort vertexCount;
    Vector3D position;
    float rotX[100];
    float rotY[100];
    float rotZ[100];
};

struct Animation {
    AnimationNode nodes[36];
    AnimationState states[10];
    byte *frameIDs;      // TODO:
    ushort frameIDCount; // TODO:
    byte field_BFAA;
    byte field_BFAB;
    ushort field_BFAC;
    ushort field_BFAE;
    ushort field_BFB0;
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

void Load_TMF_File(TMF *tmf, const char *path);
void Load_ANI_File(Animation *animation, const char *path);

#endif // !READER_H