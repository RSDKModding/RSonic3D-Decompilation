#ifndef LEVEL_H
#define LEVEL_H

#include "RetroEngine.hpp"

enum ObjectID {
    OBJECT_ID_RING,
    OBJECT_ID_SPRING,
    OBJECT_ID_RING_SPARKLE,
};

struct Object {
    uint8_t field_0;
    uint8_t field_1;
    Vector3D position;
    uint8_t gap_10[12];
    int32_t field_1C;
    int32_t field_20;
    int32_t field_24;
    int32_t field_28;
    int32_t field_2C;
    int32_t field_30;
    int32_t field_34;
};

extern InputData MGameInput;

extern int GameMode;
extern int MainGameMode;

extern float_t data_4DA248;
extern int32_t Debug;
extern int8_t PauseV;
extern int16_t data_4DA256;
extern int16_t data_4DA258;
extern int8_t TimeCount;
extern int8_t SmallSeconds;
extern int8_t Seconds;
extern int8_t Minutes;
extern int32_t data_4DA264;

extern int32_t TempObjectPos;
extern int32_t dword_41F480;

extern Vector3D data_4C9F68;

extern float_t data_4C9F74;
extern float_t data_4C9F78;
extern int32_t data_4C9F7C;

extern int32_t ObjectLoop;
extern Object LevelObjects[1100];

extern char byte_420510[12];

extern float float_420428;
extern float float_42042C;
extern float float_420430;

extern Vector3D CameraPosition;
extern float CameraCullX;
extern float CameraCullZ;

extern LMF LevelModel;
extern Texture *LevelTextureList[10];
extern Texture *stageObjectTextures[2];
extern Texture *sparkleTexture;
extern Texture *surfaceShadow;
extern Texture *surfaceMText;
extern Texture *surface3DLogo;
extern Texture *surfaceSonic;
extern Texture *surfaceTestZoneBG;
extern Texture *surfaceCharacters[5];
extern Texture *surfaceCharacters2[10];
extern Texture *TextureUnused;

extern Material material_420520;

extern TMF StageObjMdl[2];
extern TMF LogoMdl;
extern TMF BackgroundMdl;
extern TMF SonicBaseMdl;
extern TMF SonicMdl;
extern TMF BallMdl;
extern Animation SonicAni;

extern Matrix3D matrix_47A790[100][36];
extern Matrix3D array_42C590[100][36];

void InitObjectModels();
void InitZoneSurface();
void InitMTextSurface();

void sub_40F707();
void CreateObject(char a1, char a2, float x, float y, float z);
void DrawMainGameGfx();
void DrawObjects();
void DrawWorldSurface();
void Unknown_40823B(float x, float y, float z, float a4, float a5, float a6, float a7);

void Matrix_40398C(Matrix3D *matrix, Vector3D &a2, Vector3D &a3, Vector3D &a4);

void DrawModelShadow(float x, float y, float z, float magnitude, float sx, float sy, float ry);
void DrawModelSonic(float x, float y, float z, float a4);
void DrawObjectModelID(int object, float x, float y, float z, float ry, float rx, float rz);

void PlayerRotationPhysics(float *a1, float *a2, float *a3);
void ResetPlayerRotation();


void WorldMatrixTranslateXYZ(float x, float y, float z);
void WorldMatrixRotateZ(float value);
void WorldMatrixRotateY(float value);
void WorldMatrixRotateX(float value);

void WorldMatrixSetTranslateXYZ(float x, float y, float z);
void WorldMatrixSetRotateX(float x);
void WorldMatrixSetRotateY(float y);
void WorldMatrixSetRotateZ(float z);

#endif // !LEVEL_H