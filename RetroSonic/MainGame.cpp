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

Vector3D data_4C9F68;

float data_4C9F74;
float_t data_4C9F78;
int32_t data_4C9F7C;

int32_t ObjectLoop;
Object LevelObjects[1100];

char byte_420510[12];

float float_420428;
float float_42042C;
float float_420430;

LMF LevelModel;
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
    LoadTexture(stageObjectTextures[0], "Data/Objects/General/Ring.png", 0);
    Load_TMF_File(&StageObjMdl[1], "Data/Objects/General/Spring.tmf");
    LoadTexture(stageObjectTextures[1], "Data/Objects/General/Spring.png", 0);
    LoadTexture(sparkleTexture, "Data/Objects/General/Sparkle.png", 0);

    SonicAni.field_BFAB = 5;
    SonicAni.field_BFAA = 5;
    SonicAni.field_BFAC = 0;
    SonicAni.field_BFB0 = 0;
    SonicAni.field_BFAE = 0;
}

void InitZoneSurface()
{
    LoadLevelModel(&LevelModel, "Data/Levels/TestZone/Act1.lmf");

    for (int i = 0; i < 6; ++i) {
        switch (i) {
            case 0: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev01.png", 1); break;
            case 1: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev02.png", 1); break;
            case 2: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev03.png", 1); break;
            case 3: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev04.png", 1); break;
            case 4: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev05.png", 1); break;
            case 5: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev06.png", 1); break;
            case 6: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev07.png", 1); break;
            case 7: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev08.png", 1); break;
            case 8: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev09.png", 1); break;
            case 9: LoadTexture(levelSurfaceList[i], "Data/Levels/TestZone/Lev10.png", 1); break;
            default: continue;
        }
    }
}

void InitMTextSurface() { LoadTexture(surfaceMText, "Data/Title/MText.png", 0); }

void ProcessMainGame()
{
    switch (MainGameMode) {
        case 0:
            data_4C9F68.y = 10.0f;
            data_4C9F68.z = -32.0f;
            MainGameMode  = 2;

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
            RenderDevice::FlipScreen();
            break;

        case 10:
        case 11: RenderDevice::FlipScreen(); break;

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
                case 4: { // spring
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

    RenderDevice::BeginScene();
    RenderDevice::Clear(0x0000BE);
    DrawTitleScr(1);

    Unknown_40823B(Player[0].position.x, Player[0].position.y - Player[0].f_0x18.y, Player[0].position.z, data_4C9F68.x, data_4C9F68.y, data_4C9F68.z,
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
    RenderDevice::EndScene();
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
    using namespace RenderDevice;

    float c = (CameraCullX - LevelModel.startX) / 50.0f;
    float r = (CameraCullZ - LevelModel.startZ) / 50.0f;

    int minRow = CLAMP(r - 12, 0, LevelModel.rows);
    int maxRow = CLAMP(r + 12, 0, LevelModel.rows);

    int minCol = CLAMP(c - 12, 0, LevelModel.columns);
    int maxCol = CLAMP(c + 12, 0, LevelModel.columns);

    RenderDevice::SetMaterial(&material_420520);
    RenderDevice::SetRenderState(RENDER_STATE_LIGHTING, false);
    RenderDevice::SetRenderState(RENDER_STATE_ZENABLE, false);
    RenderDevice::SetTexture(0, surfaceTestZoneBG);

    WorldMatrixSetTranslateXYZ(CameraPosition.x, CameraPosition.y, CameraPosition.z);

    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
    RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, BackgroundMdl.vertices, BackgroundMdl.numVertices, BackgroundMdl.indices, BackgroundMdl.numIndices);

    RenderDevice::SetRenderState(RENDER_STATE_ZENABLE, true);
    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixIdentity);
    RenderDevice::SetTexture(0, levelSurfaceList[LevelModel.surfaceID[0]]);

    byte id = LevelModel.surfaceID[0];
    for (int s = 0; s < LevelModel.surfaceCount; ++s) {
        if (id != LevelModel.surfaceID[s]) {
            id = LevelModel.surfaceID[s];
            RenderDevice::SetTexture(0, levelSurfaceList[id]);
        }

        for (int y = minRow; y < maxRow; ++y) {
            for (int x = minCol; x < maxCol; ++x) {
                LMFMesh *tile = &LevelModel.tiles[s][y][x];
                if (tile->numVertices) {
                    for (int v = 0; v < tile->numVertices; ++v) {
                        D3DLVERTEX *vert = &tile->vertices[v];

                        Vector3D z;
                        z.x = vert->x - data_4C9F68.x;
                        z.y = vert->y - data_4C9F68.y;
                        z.z = vert->z - data_4C9F68.z;

                        Vector3D normal = z.Normalized();
                        float distance  = z.Magnitude();

                        float alpha = 1.0f;
                        if (distance < 1000.0f) {
                            if (distance > 600.0f)
                                alpha -= (distance - 600.0f) / 400.0f;
                        }
                        else {
                            if (distance >= 1000.0f)
                                alpha = 0.0f;
                        }

                        vert->color = TO_ARGB_F(alpha, tile->colors[v], tile->colors[v], tile->colors[v]);
                    }

                    RenderDevice::DrawIndexed(RENDER_FVF_LVERTEX, tile->vertices, tile->numVertices, tile->indices, tile->numIndices);
                }
            }
        }
    }

    RenderDevice::SetRenderState(RENDER_STATE_LIGHTING, true);
}

void Unknown_40823B(float x, float y, float z, float a4, float a5, float a6, float a7)
{
    using namespace RenderDevice; // temp

    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    if (PlayerPrevRotationZ != PlayerTargetRotationZ) {
        if (PlayerRotationTimerZ) {
            PlayerRotationZ =
                (double)PlayerRotationTimerZ * 0.1 * PlayerTargetRotationZ + (double)(10 - PlayerRotationTimerZ) * 0.1 * PlayerPrevRotationZ;
            if (++PlayerRotationTimerZ > 10) {
                PlayerRotationZ      = PlayerTargetRotationZ;
                PlayerPrevRotationZ  = PlayerTargetRotationZ;
                PlayerRotationTimerZ = 0;
            }
        }
        else {
            PlayerPrevRotationZ = PlayerRotationZ;
            PlayerRotationTimerZ++;
        }
    }

    if (PlayerPrevRotationX != PlayerTargetRotationX) {
        if (PlayerRotationTimerX) {
            PlayerRotationX =
                (double)PlayerRotationTimerX * 0.1 * PlayerTargetRotationX + (double)(10 - PlayerRotationTimerX) * 0.1 * PlayerPrevRotationX;
            if (++PlayerRotationTimerX > 10) {
                PlayerRotationX      = PlayerTargetRotationX;
                PlayerPrevRotationX  = PlayerTargetRotationX;
                PlayerRotationTimerX = 0;
            }
        }
        else {
            PlayerPrevRotationX = PlayerRotationX;
            PlayerRotationTimerX++;
        }
    }

    CameraPosition.x = MatrixSonicModel.m[0][0] * a4 + MatrixSonicModel.m[1][0] * a5 + MatrixSonicModel.m[2][0] * a6 + MatrixSonicModel.m[3][0];
    CameraPosition.y = MatrixSonicModel.m[0][1] * a4 + MatrixSonicModel.m[1][1] * a5 + MatrixSonicModel.m[2][1] * a6 + MatrixSonicModel.m[3][1];
    CameraPosition.z = MatrixSonicModel.m[0][2] * a4 + MatrixSonicModel.m[1][2] * a5 + MatrixSonicModel.m[2][2] * a6 + MatrixSonicModel.m[3][2];
    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    Vector3D v11;
    v11.x = MatrixSonicModel.m[0][0] * 0.0 + MatrixSonicModel.m[1][0] * 1.0 + MatrixSonicModel.m[2][0] * 0.0 + MatrixSonicModel.m[3][0];
    v11.y = MatrixSonicModel.m[0][1] * 0.0 + MatrixSonicModel.m[1][1] * 1.0 + MatrixSonicModel.m[2][1] * 0.0 + MatrixSonicModel.m[3][1];
    v11.z = MatrixSonicModel.m[0][2] * 0.0 + MatrixSonicModel.m[1][2] * 1.0 + MatrixSonicModel.m[2][2] * 0.0 + MatrixSonicModel.m[3][2];

    CameraCullX = a4 - Sin(a7) * 500.0;
    CameraCullZ = Cos(a7) * 500.0 + a6;

    Vector3D v8 = Vector3D(x, y, z);
    Vector3D v7 = Vector3D(CameraPosition.x, CameraPosition.y, CameraPosition.z);
    Matrix_40398C(&MatrixView, v7, v8, v11);

    RenderDevice::SetTransform(RENDER_TRANSFORM_VIEW, &MatrixView);
}