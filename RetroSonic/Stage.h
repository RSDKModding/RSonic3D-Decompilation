#ifndef R_STAGE_H
#define R_STAGE_H

#include "RetroEngine.hpp"

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

extern TMF stageObjectModel[2];
extern TMF logoTMF;
extern TMF backgroundTMF;
extern TMF sonicTMF2;
extern TMF sonicTMF;
extern TMF ballTMF;
extern Animation sonicAni;

extern D3DMATRIX matrix_47A790[100][36];
extern D3DMATRIX array_42C590[100][36];

void InitObjectModels();
void InitZoneSurface();
void InitMTextSurface();

#endif // !R_STAGE_H