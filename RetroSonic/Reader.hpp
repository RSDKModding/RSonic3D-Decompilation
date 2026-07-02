#ifndef READER_H
#define READER_H

#include "RetroEngine.hpp"

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

void LoadTexture(Texture **texture, const char *path, bool useTexMips);

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