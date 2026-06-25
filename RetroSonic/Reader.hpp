#pragma once
#include "RetroEngine.hpp"

struct LMF_VertexInfo {
    D3DLVERTEX *vertices;
    float *pVertexParams;
    uint16_t vertexCount;
    uint16_t _pad0;
    WORD *indexes;
    uint16_t indexCount;
    uint16_t _pad1;
};

struct LMF {
    uint8_t surfaceCount;
    uint8_t variable_1;
    uint8_t surfaceID[10];
    uint16_t variable_2;
    uint16_t variable_3;
    float variable_4;
    float variable_5;
    LMF_VertexInfo *drawList;
};

struct TMF {
    D3DTLVERTEX *vertices;
    ushort numVertices;
    ushort *indices;
    ushort numIndices;
};

struct AnimationUnknown {
    byte count;
    byte unknown1;
    ushort array_2[0x80];
    byte field_200;
    byte field_201;
};

struct AnimationFrame {
    ushort *vertexIDs;
    ushort vertexCount;
    Vector3D position;
    float rotX[100];
    float rotY[100];
    float rotZ[100];
};

struct Animation {
    AnimationFrame frames[36];
    AnimationUnknown array_AB90[10];
    byte *frameIDs;
    ushort field_BFA8;
    byte field_BFAA;
    byte field_BFAB;
    ushort field_BFAC;
    ushort field_BFAE;
    ushort field_BFB0;
};

struct FileInfo {
    void *data;
    int size;
};

IDirectDrawSurface7 *Load_PNG_File(const char *path, int a2);
void Load_LMF_File(LMF *lmf, const char *path);
void Load_TMF_File(TMF *tmf, const char *path);
void Load_ANI_File(Animation *animation, const char *path);
void LoadFile(FileInfo *file, const char *path);