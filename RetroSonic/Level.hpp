#ifndef LEVEL_H
#define LEVEL_H

#include "RetroEngine.hpp"
#include "Player.hpp"

enum ObjectTypes {
    OBJECT_NONE,
    OBJECT_RING,
    OBJECT_UNUSED,
    OBJECT_RING_SPARKLE,
    OBJECT_SPRING,
};

enum ObjectModelIDs {
    MODEL_ID_RING,
    MODEL_ID_SPRING,
    MODEL_ID_RING_SPARKLE,
};

struct Object {
    byte type;
    byte unused1;
    Vector3D position;
    int unused2;
    int unused3;
    int unused4;
    int timer;
    int unused5;
    int unused6;
    int unused7;
    int unused8;
    int unused9;
    int enabled;
};

extern InputData MGameInput;

extern int GameMode;
extern int MainGameMode;

extern int Debug;
extern int DebugEn;

extern byte PauseV;
extern byte TimeCount;
extern byte SmallSeconds;
extern byte Seconds;
extern byte Minutes;

extern int ObjectLoop;
extern int TempObjectPos;
extern Object LevelObjects[1100];

extern float RingRotationY;

extern Vector3D UnusedVector;
extern Vector3D CameraPosition;
extern Vector3D CameraTargetPosition;
extern float CameraCullX;
extern float CameraCullZ;
extern float CameraRotateX;
extern float CameraRotateY;
extern int CameraAirTimer;

extern Texture *LevelTexture[10];
extern Texture *ObjectTexture[2];
extern Texture *SparkleTexture;
extern Texture *ShadowTexture;
extern Texture *FontTexture;
extern Texture *LogoTexture;
extern Texture *UnusedTexture;
extern Texture *SonicTexture;
extern Texture *BGTexture;
extern Texture *CharacterTexture[CHARACTER_COUNT];
extern Texture *CharacterUITexture[10];

extern LMF LevelModel;
extern TMF ObjectModel[2];
extern TMF LogoModel;
extern TMF BGModel;
extern TMF SonicBaseModel;
extern TMF SonicModel;
extern TMF BallModel;

extern Animation SonicAni;
extern Material ObjectMaterial;

extern Matrix3D SonicNodeMatricesUnknown1[100][36];
extern Matrix3D SonicNodeMatricesUnknown2[100][36];

void LoadObjectAssets();
void LoadLevelAssets();
void LoadFontAssets();

void CreateObject(byte type, byte unused, float x, float y, float z);
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