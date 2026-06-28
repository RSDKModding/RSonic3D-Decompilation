#pragma once
#include "RetroEngine.hpp"

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
extern int32_t MainGameMode;
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

extern float_t data_4C9F68;
extern float_t data_4C9F6C;
extern float_t data_4C9F70;
extern float_t data_4C9F74;
extern float_t data_4C9F78;
extern int32_t data_4C9F7C;

extern int32_t ObjectLoop;
extern Object LevelObjects[1100];

extern char byte_420510[12];

extern float float_420428;
extern float float_42042C;
extern float float_420430;

extern int32_t data_4204C4;
extern int32_t data_4204C8;

extern LMF levelLMF;
extern LPDIRECTDRAWSURFACE7 levelSurfaceList[10];
extern LPDIRECTDRAWSURFACE7 stageObjectTextures[2];
extern LPDIRECTDRAWSURFACE7 sparkleTexture;
extern LPDIRECTDRAWSURFACE7 surfaceShadow;
extern LPDIRECTDRAWSURFACE7 surfaceMText;
extern LPDIRECTDRAWSURFACE7 surface3DLogo;
extern LPDIRECTDRAWSURFACE7 surfaceSonic;
extern LPDIRECTDRAWSURFACE7 surfaceTestZoneBG;
extern LPDIRECTDRAWSURFACE7 surfaceCharacters[5];
extern LPDIRECTDRAWSURFACE7 unk_420500;

extern D3DMATERIAL7 material_420520;

extern TMF StageObjMdl[2];
extern TMF LogoMdl;
extern TMF BackgroundMdl;
extern TMF SonicBaseMdl;
extern TMF SonicMdl;
extern TMF BallMdl;
extern Animation SonicAni;

extern D3DMATRIX matrix_47A790[100][36];
extern D3DMATRIX array_42C590[100][36];

void InitObjectModels();
void InitZoneSurface();
void InitMTextSurface();

void ProcessMainGame();
void ProcessTime();
void ProcessDebugMode();
void ProcessObjects();
void PauseCheck();
void sub_40F707();
void CreateObject(char a1, char a2, float x, float y, float z);
void DrawMainGameGfx();
void DrawObjects();
void DrawWorldSurface();
void Unknown_40823B(float x, float y, float z, float a4, float a5, float a6, float a7);