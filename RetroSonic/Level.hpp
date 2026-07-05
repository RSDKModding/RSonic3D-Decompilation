#ifndef LEVEL_H
#define LEVEL_H

#include "RetroEngine.hpp"
#include "Player.hpp"

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

extern float RingRotationY;

extern Vector3D UnusedVector;
extern Vector3D CameraEye;
extern Vector3D CameraPosition;
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

extern Animator SonicAni;
extern Material RenderMaterial;

extern Matrix3D SonicNodeMatrixPositionStore[100][36];
extern Matrix3D SonicNodeMatrixNormalStore[100][36];

void LoadObjectAssets();
void LoadLevelAssets();
void LoadFontAssets();

void DrawMainGameGfx();
void DrawObjects();
void DrawLevelModel();

void UpdateCamera(float x, float y, float z, float cx, float cy, float cz, float ry);
void CameraLook(Matrix3D *matrix, Vector3D &eye, Vector3D &target, Vector3D &rotation);

#endif // !LEVEL_H