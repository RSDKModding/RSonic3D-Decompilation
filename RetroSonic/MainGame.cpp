#include "RetroEngine.hpp"

InputData MGameInput;
int32_t MainGameMode;
float_t data_4DA248;
int32_t Debug;
int8_t PauseV;
int16_t data_4DA256;
int16_t data_4DA258;
int8_t TimeCount;
int8_t SmallSeconds;
int8_t Seconds;
int8_t Minutes;
int32_t data_4DA264;

int32_t TempObjectPos = 232;
int32_t dword_41F480  = 1;

float_t data_4C9F68;
float_t data_4C9F6C;
float_t data_4C9F70;
float_t data_4C9F74;
float_t data_4C9F78;
int32_t data_4C9F7C;

int32_t ObjectLoop;
Object LevelObjects[1100];

char byte_420510[12];

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

TMF StageObjMdl[2];
TMF LogoMdl;
TMF BackgroundMdl;
TMF SonicBaseMdl;
TMF SonicMdl;
TMF BallMdl;
Animation SonicAni;

D3DMATRIX matrix_47A790[100][36];
D3DMATRIX array_42C590[100][36]; // TODO: Correct size?

void InitObjectModels()
{
    Load_TMF_File(&LogoMdl, "Data/Title/Logo.tmf");
    Load_TMF_File(&BackgroundMdl, "Data/Levels/TestZone/BG.tmf");
    Load_TMF_File(&SonicBaseMdl, "Data/Characters/Sonic.tmf");
    Load_TMF_File(&SonicMdl, "Data/Characters/Sonic.tmf");
    Load_TMF_File(&BallMdl, "Data/Characters/Ball.tmf");
    Load_ANI_File(&SonicAni, "Data/Characters/Sonic.ani");
    MightBeSonicAnim_406432();

    Load_TMF_File(&StageObjMdl[0], "Data/Objects/General/Ring.tmf");
    stageObjectTextures[0] = Load_PNG_File("Data/Objects/General/Ring.png", 0);
    Load_TMF_File(&StageObjMdl[1], "Data/Objects/General/Spring.tmf");
    stageObjectTextures[1] = Load_PNG_File("Data/Objects/General/Spring.png", 0);
    sparkleTexture         = Load_PNG_File("Data/Objects/General/Sparkle.png", 0);

    SonicAni.field_BFAB = 5;
    SonicAni.field_BFAA = 5;
    SonicAni.field_BFAC = 0;
    SonicAni.field_BFB0 = 0;
    SonicAni.field_BFAE = 0;
}

void InitZoneSurface()
{
    LoadLevelModel(&levelLMF, "Data/Levels/TestZone/Act1.lmf");

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

void ProcessMainGame()
{
    switch (MainGameMode) {
        case 0:
            data_4C9F6C  = 10.0f;
            data_4C9F70  = -32.0f;
            MainGameMode = 2;

            for (ObjectLoop = 0; ObjectLoop < 6; ++ObjectLoop) {
                LevelObjects[ObjectLoop].field_0    = 1;
                LevelObjects[ObjectLoop].field_34   = 1;
                LevelObjects[ObjectLoop].position.x = (double)ObjectLoop * -8.0 - 10.0;
                LevelObjects[ObjectLoop].position.z = -20.0;
                LevelObjects[ObjectLoop].position.y = 5.0;
            }

            for (ObjectLoop = 10; ObjectLoop < 14; ++ObjectLoop) {
                LevelObjects[ObjectLoop].field_0    = 1;
                LevelObjects[ObjectLoop].field_34   = 1;
                LevelObjects[ObjectLoop].position.z = (double)ObjectLoop * 8.0 - 60.0;
                LevelObjects[ObjectLoop].position.x = -50.0;
                LevelObjects[ObjectLoop].position.y = 12.0;
            }

            for (ObjectLoop = 14; ObjectLoop < 15; ++ObjectLoop) {
                LevelObjects[ObjectLoop].field_0    = 4;
                LevelObjects[ObjectLoop].field_34   = 1;
                LevelObjects[ObjectLoop].position.z = 50.0;
                LevelObjects[ObjectLoop].position.x = -80.0;
                LevelObjects[ObjectLoop].position.y = 1.5;
            }
            break;

        case 1: data_4DA264 = 0; break;

        case 2:
            ProcessPlayerInput();
            PauseCheck();

            if (!PauseV) {
                if (Debug) {
                    ProcessDebugMode();
                }
                else {
                    ProcessPlayerMovement();
                    ProcessObjects();
                    ProcessTime();

                    for (ObjectLoop = 0; ObjectLoop < 2000; ++ObjectLoop) {
                        // nothing here
                    }

                    ProcessPlayerAnimationLMC();
                }

                PNumber     = 0;
                data_4DA256 = 0;
            }

            DrawMainGameGfx();
            FlipScreen();
            break;

        case 10:
        case 11: FlipScreen(); break;

        default: break;
    }
}

void ProcessTime()
{
    if (++TimeCount == 6) {
        SmallSeconds += 10;
        TimeCount = 0;
    }

    if (SmallSeconds == 100) {
        if (++Seconds == 60) {
            ++Minutes;
            Seconds = 0;
        }
        SmallSeconds = 0;
    }
}

void ProcessDebugMode()
{
    // chillin
}

void ProcessObjects()
{
    for (ObjectLoop = 0; ObjectLoop < 1100; ++ObjectLoop) {
        if (LevelObjects[ObjectLoop].field_34 > 0) {
            switch (LevelObjects[ObjectLoop].field_0) {
                case 1: {
                    Vector3D position;
                    position.z = LevelObjects[ObjectLoop].position.z - Player[0].position.z;
                    position.y = LevelObjects[ObjectLoop].position.y - Player[0].position.y + Player[0].f_0x18.y;
                    position.x = LevelObjects[ObjectLoop].position.x - Player[0].position.x;

                    if (position.Magnitude() < 6.0)
                        LevelObjects[ObjectLoop].field_0 = 3;
                    break;
                }
                case 3: {
                    if (++LevelObjects[ObjectLoop].field_1C > 15) {
                        LevelObjects[ObjectLoop].field_1C = 0;
                        LevelObjects[ObjectLoop].field_0  = 0;
                    }
                    break;
                }
                case 4: {
                    Vector3D position;
                    position.z = LevelObjects[ObjectLoop].position.z - Player[0].position.z;
                    position.y = LevelObjects[ObjectLoop].position.y - Player[0].position.y + Player[0].f_0x18.y;
                    position.x = LevelObjects[ObjectLoop].position.x - Player[0].position.x;

                    if (position.Magnitude() < 6.0) {
                        Player[PNumber].velocity.y = 4.0;
                        Player[PNumber].f_0x44     = 1;
                    }
                    break;
                }
            }
        }
    }

    data_4DA248 += 0.050000001;
    if (RETRO_PI + RETRO_PI < data_4DA248)
        data_4DA248 -= (RETRO_PI + RETRO_PI);
}

void PauseCheck()
{
    if (MGameInput.start == TRUE) {
        if (PauseV) {
            if (PauseV == 1)
                PauseV = 0;
        }
        else {
            PauseV = 1;
        }
    }
}

void sub_40F707()
{
    data_4DA258 = data_4DA256;
    data_4DA256 = 0;
}

void CreateObject(char a1, char a2, float x, float y, float z)
{
    if (LevelObjects[TempObjectPos].field_0 != 0) {
        if (++TempObjectPos >= 1100)
            TempObjectPos = 1000;
    }

    LevelObjects[TempObjectPos].field_0    = a1;
    LevelObjects[TempObjectPos].field_34   = 1;
    LevelObjects[TempObjectPos].field_1    = a2;
    LevelObjects[TempObjectPos].position.x = x;
    LevelObjects[TempObjectPos].position.y = y;
    LevelObjects[TempObjectPos].position.z = z;
    LevelObjects[TempObjectPos].field_1C   = 0;
    LevelObjects[TempObjectPos].field_20   = 0;
    LevelObjects[TempObjectPos].field_24   = 0;
    LevelObjects[TempObjectPos].field_28   = 0;
    LevelObjects[TempObjectPos].field_2C   = 0;
    LevelObjects[TempObjectPos].field_30   = 0;
}

void DrawMainGameGfx()
{
    float a2;    // [esp+0h] [ebp-18h]
    float a2a;   // [esp+0h] [ebp-18h]
    float a3;    // [esp+4h] [ebp-14h]
    float value; // [esp+Ch] [ebp-Ch]
    float v4;    // [esp+14h] [ebp-4h]
    float a7a;   // [esp+14h] [ebp-4h]

    BeginScene();
    Render_ClearScreen(190);
    DrawTitleScr(1);

    Unknown_40823B(Player[0].position.x, Player[0].position.y - Player[0].f_0x18.y, Player[0].position.z, data_4C9F68, data_4C9F6C, data_4C9F70,
                   data_4C9F74);
    DrawWorldSurface();

    if (Debug) {
        value = Player[0].position.x * 0.1;
        a3    = Player[0].position.y + 4.0;
        DrawObjectModelID(0, Player[0].position.x, a3, Player[0].position.z, value, 0.0, 0.0);
        a7a = Player[0].position.x * 0.1;
        a2a = Player[0].position.y + 4.0;
        DrawModelShadow(Player[0].position.x, a2a, Player[0].position.z, -30.0, 2.0, 1.0, a7a);
    }
    else {
        v4 = Player[PNumber].f_0x30 * -1.0 + 3.1415927;
        DrawModelSonic(Player[0].position.x, Player[0].position.y, Player[0].position.z, v4);
    }

    DrawObjects();
    EndScene();
}

void DrawObjects()
{
    for (ObjectLoop = 0; ObjectLoop < 1100; ++ObjectLoop) {
        if (LevelObjects[ObjectLoop].field_34 > 0) {
            switch (LevelObjects[ObjectLoop].field_0) {
                case 1:
                    DrawObjectModelID(OBJECT_ID_RING, LevelObjects[ObjectLoop].position.x, LevelObjects[ObjectLoop].position.y,
                                      LevelObjects[ObjectLoop].position.z, data_4DA248, 0.0f, 0.0f);
                    DrawModelShadow(LevelObjects[ObjectLoop].position.x, LevelObjects[ObjectLoop].position.y, LevelObjects[ObjectLoop].position.z,
                                    -30.0f, 2.0f, 1.0f, data_4DA248);
                    break;

                case 3:
                    switch (LevelObjects[ObjectLoop].field_1C >> 2) {
                        case 0:
                            DrawObjectModelID(OBJECT_ID_RING_SPARKLE, LevelObjects[ObjectLoop].position.x, LevelObjects[ObjectLoop].position.y,
                                              LevelObjects[ObjectLoop].position.z, 0.0, 0.0, 0.0f);
                            break;

                        case 1:
                            DrawObjectModelID(OBJECT_ID_RING_SPARKLE, LevelObjects[ObjectLoop].position.x, LevelObjects[ObjectLoop].position.y,
                                              LevelObjects[ObjectLoop].position.z, 0.0, 0.0, 1.5700001f);
                            break;

                        case 2:
                            DrawObjectModelID(OBJECT_ID_RING_SPARKLE, LevelObjects[ObjectLoop].position.x, LevelObjects[ObjectLoop].position.y,
                                              LevelObjects[ObjectLoop].position.z, 0.0f, 0.0f, RETRO_PI);
                            break;

                        case 3:
                            DrawObjectModelID(OBJECT_ID_RING_SPARKLE, LevelObjects[ObjectLoop].position.x, LevelObjects[ObjectLoop].position.y,
                                              LevelObjects[ObjectLoop].position.z, 0.0f, 0.0f, RETRO_PI + 1.5700001f);
                            break;

                        default: continue;
                    }
                    break;

                case 4:
                    DrawObjectModelID(OBJECT_ID_SPRING, LevelObjects[ObjectLoop].position.x, LevelObjects[ObjectLoop].position.y,
                                      LevelObjects[ObjectLoop].position.z, 0.0f, 0.0f, 0.0f);
                    break;
            }
        }
    }
}

void DrawWorldSurface()
{
    double v0;   // st7
    Vector3D v1; // [esp+4h] [ebp-44h] BYREF
    int k;       // [esp+10h] [ebp-38h]
    int j;       // [esp+14h] [ebp-34h]
    int l;       // [esp+18h] [ebp-30h]
    float v5;    // [esp+1Ch] [ebp-2Ch]
    Vector3D z;  // [esp+20h] [ebp-28h] BYREF
    int v8;      // [esp+30h] [ebp-18h]
    int v9;      // [esp+34h] [ebp-14h]
    int v10;     // [esp+38h] [ebp-10h]
    int i;       // [esp+3Ch] [ebp-Ch]
    int v12;     // [esp+40h] [ebp-8h]
    int v13;     // [esp+44h] [ebp-4h]

    // callback((int)&z);

    v13 = (__int64)((data_4C9D50 - levelLMF.variable_4) / 50.0) - 12;
    v12 = v13 + 24;
    v9  = (__int64)((data_4C9D58 - levelLMF.variable_5) / 50.0) - 12;
    v8  = v9 + 24;
    if (v13 < 0)
        v13 = 0;
    if (v13 > levelLMF.variable_2)
        v13 = levelLMF.variable_2;
    if (v12 < 0)
        v12 = 0;
    if (v12 > levelLMF.variable_2)
        v12 = levelLMF.variable_2;
    if (v9 < 0)
        v9 = 0;
    if (v9 > levelLMF.variable_3)
        v9 = levelLMF.variable_3;
    if (v8 < 0)
        v8 = 0;
    if (v8 > levelLMF.variable_3)
        v8 = levelLMF.variable_3;

    D3DDevice->SetMaterial(&material_420520);
    D3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, 0);
    D3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
    D3DDevice->SetTexture(0, surfaceTestZoneBG);

    Matrix_40893B(vector_4C9D74.x, vector_4C9D74.y, vector_4C9D74.z);

    D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matrixBackgroundTransform);
    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (0x002 | 0x010 | 0x100), BackgroundMdl.vertices, BackgroundMdl.numVertices,
                                    BackgroundMdl.indices, BackgroundMdl.numIndices, 0);

    D3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
    D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld);
    D3DDevice->SetTexture(0, levelSurfaceList[levelLMF.surfaceID[0]]);

    uint8_t v7 = levelLMF.surfaceID[0];
    for (i = 0; i < levelLMF.surfaceCount; ++i) {
        if (v7 != levelLMF.surfaceID[i]) {
            v7 = levelLMF.surfaceID[i];
            D3DDevice->SetTexture(0, levelSurfaceList[v7]);
        }

        v10 = levelLMF.variable_2 * v9 + v13 + i * levelLMF.variable_3 * levelLMF.variable_2;
        for (j = v9; j < v8; ++j) {
            for (k = v13; k < v12; ++k) {
                if (levelLMF.drawList[v10].vertexCount) {
                    for (l = 0; l < levelLMF.drawList[v10].vertexCount; ++l) {
                        z.x = levelLMF.drawList[v10].vertices[l].x - data_4C9F68;
                        z.y = levelLMF.drawList[v10].vertices[l].y - data_4C9F6C;
                        z.z = levelLMF.drawList[v10].vertices[l].z - data_4C9F70;

                        // this is just here
                        z.Normalized();

                        v5  = z.Magnitude();
                        z.x = levelLMF.drawList[v10].pVertexParams[l];
                        if (v5 <= 600.0 || v5 >= 1000.0) {
                            if (v5 < 1000.0)
                                levelLMF.drawList[v10].vertices[l].color = (__int64)(z.x * 255.0) | ((unsigned int)(__int64)(z.x * 255.0) << 8)
                                                                           | ((unsigned int)(__int64)(z.x * 255.0) << 16) | 0xFF000000;
                            else
                                levelLMF.drawList[v10].vertices[l].color = (__int64)(z.x * 255.0) | ((unsigned int)(__int64)(z.x * 255.0) << 8)
                                                                           | ((unsigned int)(__int64)(z.x * 255.0) << 16);
                        }
                        else {
                            v0                                       = 1.0 - (v5 - 600.0) / 400.0;
                            v5                                       = v0;
                            levelLMF.drawList[v10].vertices[l].color = (__int64)(z.x * 255.0) | ((unsigned int)(__int64)(z.x * 255.0) << 8)
                                                                       | ((unsigned int)(__int64)(z.x * 255.0) << 16)
                                                                       | ((unsigned int)(__int64)(v0 * 255.0) << 24);
                        }
                    }

                    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (0x002 | 0x020 | 0x040 | 0x080 | 0x100), levelLMF.drawList[v10].vertices,
                                                    levelLMF.drawList[v10].vertexCount, levelLMF.drawList[v10].indexes,
                                                    levelLMF.drawList[v10].indexCount, 0);
                }
                ++v10;
            }
            v10 += levelLMF.variable_2 - (v12 - v13);
        }
    }

    D3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, 1);
}

void Unknown_40823B(float x, float y, float z, float a4, float a5, float a6, float a7)
{
    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));

    if (data_4C9D6C != data_4C9D5C) {
        if (data_4204C8) {
            data_4C9D64 = (double)data_4204C8 * 0.1 * data_4C9D5C + (double)(10 - data_4204C8) * 0.1 * data_4C9D6C;
            if (++data_4204C8 > 10) {
                data_4C9D64 = data_4C9D5C;
                data_4C9D6C = data_4C9D5C;
                data_4204C8 = 0;
            }
        }
        else {
            data_4C9D6C = data_4C9D64;
            ++data_4204C8;
        }
    }

    if (data_4C9D70 != data_4C9D60) {
        if (data_4204C4) {
            data_4C9D68 = (double)data_4204C4 * 0.1 * data_4C9D60 + (double)(10 - data_4204C4) * 0.1 * data_4C9D70;
            if (++data_4204C4 > 10) {
                data_4C9D68 = data_4C9D60;
                data_4C9D70 = data_4C9D60;
                data_4204C4 = 0;
            }
        }
        else {
            data_4C9D70 = data_4C9D68;
            ++data_4204C4;
        }
    }

    vector_4C9D74.x = matSonicMdl.m[0][0] * a4 + matSonicMdl.m[1][0] * a5 + matSonicMdl.m[2][0] * a6 + matSonicMdl.m[3][0];
    vector_4C9D74.y = matSonicMdl.m[0][1] * a4 + matSonicMdl.m[1][1] * a5 + matSonicMdl.m[2][1] * a6 + matSonicMdl.m[3][1];
    vector_4C9D74.z = matSonicMdl.m[0][2] * a4 + matSonicMdl.m[1][2] * a5 + matSonicMdl.m[2][2] * a6 + matSonicMdl.m[3][2];
    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));

    Vector3D v11;
    v11.x = matSonicMdl.m[0][0] * 0.0 + matSonicMdl.m[1][0] * 1.0 + matSonicMdl.m[2][0] * 0.0 + matSonicMdl.m[3][0];
    v11.y = matSonicMdl.m[0][1] * 0.0 + matSonicMdl.m[1][1] * 1.0 + matSonicMdl.m[2][1] * 0.0 + matSonicMdl.m[3][1];
    v11.z = matSonicMdl.m[0][2] * 0.0 + matSonicMdl.m[1][2] * 1.0 + matSonicMdl.m[2][2] * 0.0 + matSonicMdl.m[3][2];

    data_4C9D50 = a4 - Sin(a7) * 500.0;
    data_4C9D58 = Cos(a7) * 500.0 + a6;

    Vector3D v8 = Vector3D(x, y, z);
    Vector3D v7 = Vector3D(vector_4C9D74.x, vector_4C9D74.y, vector_4C9D74.z);
    Matrix_40398C(&matView, v7, v8, v11);

    IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_VIEW, &matView);
}