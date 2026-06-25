#include "RetroEngine.hpp"

float float_420428;
float float_42042C;
float float_420430;

int32_t data_4204C4;
int32_t data_4204C8;

LMF levelLMF;
LPDIRECTDRAWSURFACE7 levelSurfaceList[10];
LPDIRECTDRAWSURFACE7 stageObjectTextures[2];
LPDIRECTDRAWSURFACE7 sparkleTexture;
LPDIRECTDRAWSURFACE7 surfaceShadow;
LPDIRECTDRAWSURFACE7 surfaceMText;
LPDIRECTDRAWSURFACE7 surface3DLogo;
LPDIRECTDRAWSURFACE7 surfaceSonic;
LPDIRECTDRAWSURFACE7 surfaceTestZoneBG;
LPDIRECTDRAWSURFACE7 surfaceCharacters[5];
LPDIRECTDRAWSURFACE7 unk_420500;

D3DMATERIAL7 material_420520 = {
    { { 0.0, 0.0, 0.0, 0.0 } }, { { 0.0, 0.0, 0.0, 0.0 } }, { { 0.0, 0.0, 0.0, 0.0 } }, { { 0.0, 0.0, 0.0, 0.0 } }, { 0.0 }
};

TMF stageObjectModel[2];
TMF logoTMF;
TMF backgroundTMF;
TMF sonicTMF2;
TMF sonicTMF;
TMF ballTMF;
Animation sonicAni;

D3DMATRIX matrix_47A790[100][36];
D3DMATRIX array_42C590[100][36]; // TODO: Correct size?

void InitObjectModels()
{
    Load_TMF_File(&logoTMF, "Data/Title/Logo.tmf");
    Load_TMF_File(&backgroundTMF, "Data/Levels/TestZone/BG.tmf");
    Load_TMF_File(&sonicTMF2, "Data/Characters/Sonic.tmf");
    Load_TMF_File(&sonicTMF, "Data/Characters/Sonic.tmf");
    Load_TMF_File(&ballTMF, "Data/Characters/Ball.tmf");
    Load_ANI_File(&sonicAni, "Data/Characters/Sonic.ani");
    MightBeSonicAnim_406432();

    Load_TMF_File(&stageObjectModel[0], "Data/Objects/General/Ring.tmf");
    stageObjectTextures[0] = Load_PNG_File("Data/Objects/General/Ring.png", 0);
    Load_TMF_File(&stageObjectModel[1], "Data/Objects/General/Spring.tmf");
    stageObjectTextures[1] = Load_PNG_File("Data/Objects/General/Spring.png", 0);
    sparkleTexture         = Load_PNG_File("Data/Objects/General/Sparkle.png", 0);

    sonicAni.field_BFAB = 5;
    sonicAni.field_BFAA = 5;
    sonicAni.field_BFAC = 0;
    sonicAni.field_BFB0 = 0;
    sonicAni.field_BFAE = 0;
}

void InitZoneSurface()
{
    Load_LMF_File(&levelLMF, "Data/Levels/TestZone/Act1.lmf");

    for (int i = 0; i < 6; ++i) {
        switch (i) {
            case 0: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev01.png", 1); break;
            case 1: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev02.png", 1); break;
            case 2: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev03.png", 1); break;
            case 3: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev04.png", 1); break;
            case 4: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev05.png", 1); break;
            case 5: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev06.png", 1); break;
            case 6: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev07.png", 1); break;
            case 7: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev08.png", 1); break;
            case 8: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev09.png", 1); break;
            case 9: levelSurfaceList[i] = Load_PNG_File("Data/Levels/TestZone/Lev10.png", 1); break;
            default: continue;
        }
    }
}

void InitMTextSurface() { surfaceMText = Load_PNG_File("Data/Title/MText.png", 0); }