#include "RetroEngine.hpp"

InputData MGameInput;

int GameMode;
int MainGameMode;

int Debug   = false; // Are we currently in debugmode?
int DebugEn = true;  // Will the game allow us to use debugmode?

byte PauseV;
byte TimeCount;
byte SmallSeconds;
byte Seconds;
byte Minutes;

int ObjectLoop;
int TempObjectPos = 232;
Object LevelObjects[1100];

float RingRotationY;

Vector3D UnusedVector;
Vector3D CameraPosition;
Vector3D CameraTargetPosition;
float CameraCullX;
float CameraCullZ;
float CameraRotateX;
float CameraRotateY;
int CameraAirTimer;

Texture *LevelTexture[10];
Texture *ObjectTexture[2];
Texture *SparkleTexture;
Texture *ShadowTexture;
Texture *FontTexture;
Texture *LogoTexture;
Texture *UnusedTexture;
Texture *SonicTexture;
Texture *BGTexture;
Texture *CharacterTexture[CHARACTER_COUNT];
Texture *CharacterUITexture[10];

LMF LevelModel;
TMF ObjectModel[2];
TMF LogoModel;
TMF BGModel;
TMF SonicBaseModel;
TMF SonicModel;
TMF BallModel;

Animation SonicAni;
Material ObjectMaterial;

// i'm not sure what these are for
// these are set but never read
Matrix3D SonicNodeMatricesUnknown1[100][36];
Matrix3D SonicNodeMatricesUnknown2[100][36];

void LoadObjectAssets()
{
    Load_TMF_File(&LogoModel, "Data/Title/Logo.tmf");
    Load_TMF_File(&BGModel, "Data/Levels/TestZone/BG.tmf");
    Load_TMF_File(&SonicBaseModel, "Data/Characters/Sonic.tmf");
    Load_TMF_File(&SonicModel, "Data/Characters/Sonic.tmf");
    Load_TMF_File(&BallModel, "Data/Characters/Ball.tmf");
    Load_ANI_File(&SonicAni, "Data/Characters/Sonic.ani");
    MightBeSonicAnim_406432();

    Load_TMF_File(&ObjectModel[0], "Data/Objects/General/Ring.tmf");
    LoadTexture(&ObjectTexture[0], "Data/Objects/General/Ring.png", false);

    Load_TMF_File(&ObjectModel[1], "Data/Objects/General/Spring.tmf");
    LoadTexture(&ObjectTexture[1], "Data/Objects/General/Spring.png", false);

    LoadTexture(&SparkleTexture, "Data/Objects/General/Sparkle.png", false);

    SonicAni.field_BFAB = 5;
    SonicAni.field_BFAA = 5;
    SonicAni.field_BFAC = 0;
    SonicAni.field_BFB0 = 0;
    SonicAni.field_BFAE = 0;
}

void LoadLevelAssets()
{
    LoadLevelModel(&LevelModel, "Data/Levels/TestZone/Act1.lmf");

    for (int i = 0; i < 6; ++i) {
        switch (i) {
            case 0: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev01.png", true); break;
            case 1: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev02.png", true); break;
            case 2: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev03.png", true); break;
            case 3: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev04.png", true); break;
            case 4: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev05.png", true); break;
            case 5: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev06.png", true); break;
            case 6: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev07.png", true); break;
            case 7: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev08.png", true); break;
            case 8: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev09.png", true); break;
            case 9: LoadTexture(&LevelTexture[i], "Data/Levels/TestZone/Lev10.png", true); break;
            default: continue;
        }
    }
}

void LoadFontAssets() { LoadTexture(&FontTexture, "Data/Title/MText.png", false); }

void CreateObject(byte type, byte unused, float x, float y, float z)
{
    if (LevelObjects[TempObjectPos].type != OBJECT_NONE) {
        if (++TempObjectPos >= 1100)
            TempObjectPos = 1000;
    }

    MEM_ZERO(&LevelObjects[TempObjectPos], sizeof(Object));

    LevelObjects[TempObjectPos].type     = type;
    LevelObjects[TempObjectPos].unused1  = unused;
    LevelObjects[TempObjectPos].position = { x, y, z };
    LevelObjects[TempObjectPos].enabled  = true;
}

void DrawMainGameGfx()
{
    BeginScene();
    ClearScreen(0x0000BE);
    SetSceneRenderState(1);

    PlayerObject *player  = &Player[PNumber];
    PlayerObject *player1 = &Player[0];

    float x = player1->position.x;
    float y = player1->position.y;
    float z = player1->position.z;

    Unknown_40823B(x, y - player1->collisionPos.y, z, CameraTargetPosition.x, CameraTargetPosition.y, CameraTargetPosition.z, CameraRotateY);
    DrawWorldSurface();

    if (Debug) {
        DrawObjectModelID(MODEL_ID_RING, x, y + 4.0f, z, x * 0.1f, 0.0f, 0.0f);
        DrawModelShadow(x, y + 4.0f, z, -30.0f, 2.0f, 1.0f, x * 0.1f);
    }
    else {
        DrawModelSonic(x, y, z, -player->rotationY + RSDK_PI);
    }

    DrawObjects();
    EndScene();
}

void DrawObjects()
{
    for (ObjectLoop = 0; ObjectLoop < 1100; ++ObjectLoop) {
        Object *object = &LevelObjects[ObjectLoop];

        float x = object->position.x;
        float y = object->position.y;
        float z = object->position.z;

        if (object->enabled > 0) {
            switch (object->type) {
                case 1:
                    DrawObjectModelID(MODEL_ID_RING, x, y, z, RingRotationY, 0.0f, 0.0f);
                    DrawModelShadow(x, y, z, -30.0f, 2.0f, 1.0f, RingRotationY);
                    break;

                case 3:
                    switch (object->timer >> 2) {
                        case 0: DrawObjectModelID(MODEL_ID_RING_SPARKLE, x, y, z, 0.0, 0.0, 0.0f); break;
                        case 1: DrawObjectModelID(MODEL_ID_RING_SPARKLE, x, y, z, 0.0, 0.0, 1.57f); break;
                        case 2: DrawObjectModelID(MODEL_ID_RING_SPARKLE, x, y, z, 0.0f, 0.0f, RSDK_PI); break;
                        case 3: DrawObjectModelID(MODEL_ID_RING_SPARKLE, x, y, z, 0.0f, 0.0f, RSDK_PI + 1.57f); break;
                        default: continue;
                    }
                    break;

                case 4: DrawObjectModelID(MODEL_ID_SPRING, x, y, z, 0.0f, 0.0f, 0.0f); break;
            }
        }
    }
}

void DrawWorldSurface()
{
    float c = (CameraCullX - LevelModel.startX) / 50.0f;
    float r = (CameraCullZ - LevelModel.startZ) / 50.0f;

    int minRow = CLAMP(r - 12, 0, LevelModel.rows);
    int maxRow = CLAMP(r + 12, 0, LevelModel.rows);

    int minCol = CLAMP(c - 12, 0, LevelModel.columns);
    int maxCol = CLAMP(c + 12, 0, LevelModel.columns);

    SetRenderMaterial(&ObjectMaterial);
    SetRenderState(RENDER_STATE_LIGHTING, false);
    SetRenderState(RENDER_STATE_ZENABLE, false);
    SetRenderTexture(0, BGTexture);

    WorldMatrixSetTranslateXYZ(CameraPosition.x, CameraPosition.y, CameraPosition.z);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
    DrawIndexedPrimitive(RENDER_FVF_VERTEX, BGModel.vertices, BGModel.numVertices, BGModel.indices, BGModel.numIndices);

    SetRenderState(RENDER_STATE_ZENABLE, true);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixIdentity);
    SetRenderTexture(0, LevelTexture[LevelModel.surfaceID[0]]);

    byte id = LevelModel.surfaceID[0];
    for (int s = 0; s < LevelModel.surfaceCount; ++s) {
        if (id != LevelModel.surfaceID[s]) {
            id = LevelModel.surfaceID[s];
            SetRenderTexture(0, LevelTexture[id]);
        }

        for (int y = minRow; y < maxRow; ++y) {
            for (int x = minCol; x < maxCol; ++x) {
                LMFMesh *tile = &LevelModel.tiles[s][y][x];
                if (tile->numVertices) {
                    for (int v = 0; v < tile->numVertices; ++v) {
                        LVertex *vert = &tile->vertices[v];

                        Vector3D z;
                        z.x = vert->x - CameraTargetPosition.x;
                        z.y = vert->y - CameraTargetPosition.y;
                        z.z = vert->z - CameraTargetPosition.z;

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

                        vert->color = PACK_ARGB_F(alpha, tile->colors[v], tile->colors[v], tile->colors[v]);
                    }

                    DrawIndexedPrimitive(RENDER_FVF_LVERTEX, tile->vertices, tile->numVertices, tile->indices, tile->numIndices);
                }
            }
        }
    }

    SetRenderState(RENDER_STATE_LIGHTING, true);
}

void Unknown_40823B(float x, float y, float z, float a4, float a5, float a6, float a7)
{
    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    if (PlayerPrevRotationZ != PlayerTargetRotationZ) {
        if (PlayerRotationTimerZ) {
            PlayerRotationZ = PlayerRotationTimerZ * (0.1f * PlayerTargetRotationZ) + (10 - PlayerRotationTimerZ) * (0.1f * PlayerPrevRotationZ);
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
            PlayerRotationX = PlayerRotationTimerX * (0.1f * PlayerTargetRotationX) + (10 - PlayerRotationTimerX) * (0.1f * PlayerPrevRotationX);
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

    Matrix3D *matrix = &MatrixSonicModel;
    CameraPosition.x = matrix->m[0][0] * a4 + matrix->m[1][0] * a5 + matrix->m[2][0] * a6 + matrix->m[3][0];
    CameraPosition.y = matrix->m[0][1] * a4 + matrix->m[1][1] * a5 + matrix->m[2][1] * a6 + matrix->m[3][1];
    CameraPosition.z = matrix->m[0][2] * a4 + matrix->m[1][2] * a5 + matrix->m[2][2] * a6 + matrix->m[3][2];
    memcpy(matrix, &MatrixIdentity, sizeof(*matrix));

    Vector3D v11;
    v11.x = matrix->m[0][0] * 0.0f + matrix->m[1][0] * 1.0f + matrix->m[2][0] * 0.0f + matrix->m[3][0];
    v11.y = matrix->m[0][1] * 0.0f + matrix->m[1][1] * 1.0f + matrix->m[2][1] * 0.0f + matrix->m[3][1];
    v11.z = matrix->m[0][2] * 0.0f + matrix->m[1][2] * 1.0f + matrix->m[2][2] * 0.0f + matrix->m[3][2];

    CameraCullX = a4 - Sin(a7) * 500.0f;
    CameraCullZ = Cos(a7) * 500.0f + a6;

    Vector3D v8 = { x, y, z };
    Vector3D v7 = CameraPosition;
    Matrix_40398C(&MatrixView, v7, v8, v11);

    SetRenderTransform(RENDER_TRANSFORM_VIEW, &MatrixView);
}

void DrawModelShadow(float x, float y, float z, float magnitude, float sx, float sy, float ry)
{
    Vector3D origin    = { x, y + 1.0f, z };
    Vector3D direction = { 0.0f, 1.0f, 0.0f };

    float point[3];
    float colist[9];

    float c = (x - LevelModel.startX) / 50.0f;
    float r = (z - LevelModel.startZ) / 50.0f;

    int minRow = CLAMP(r - 2, 0, LevelModel.rows);
    int maxRow = CLAMP(r + 2, 0, LevelModel.rows);

    int minCol = CLAMP(c - 2, 0, LevelModel.columns);
    int maxCol = CLAMP(c + 2, 0, LevelModel.columns);

    Collided = false;

    LVertex vertices[4];
    MEM_ZERO(vertices, sizeof(vertices));

    ushort indices[] = { 0, 1, 2, 1, 3, 2 };

    float alpha = 0.5f;
    for (int row = minRow; row < maxRow; ++row) {
        for (int col = minCol; col < maxCol; ++col) {
            if (LCollision[row][col]->rayCollision(&origin[0], &direction[0], false, 0.0f, magnitude)) {
                LCollision[row][col]->getCollisionPoint(point);
                x = point[0];
                y = point[1] + sy * 0.01f;
                z = point[2];

                LCollision[row][col]->getCollidingTriangles(colist, NULL);
                Collided = true;
            }
        }
    }

    if (Collided == true) {
        origin.y = (origin.y - 2.0f) - y;
        origin.x = magnitude * -0.75f;
        origin.z = magnitude * -0.5f;

        Vector3D ab = { colist[3] - colist[0], colist[4] - colist[1], colist[5] - colist[2] };
        Vector3D ac = { colist[6] - colist[0], colist[7] - colist[1], colist[8] - colist[2] };

        if (origin.y > origin.x)
            alpha = 0.5f - (origin.y - origin.x) / (-magnitude - origin.x) * 0.5f;

        if (origin.y > origin.z) {
            sx -= (origin.y - origin.z) / (-magnitude - origin.z) * sx;
            sy -= (origin.y - origin.z) / (-magnitude - origin.z) * sy;
        }

        Vector3D normal = CrossProduct(ab, ac).Normalized();
        Color color     = PACK_ARGB_F(alpha, 255, 255, 255);

        vertices[0] = { { -sx, 0.0f, sy }, color, 0, 0.05f, 0.05f };
        vertices[1] = { { sx, 0.0f, sy }, color, 0, 0.99f, 0.05f };
        vertices[2] = { { -sx, 0.0f, -sy }, color, 0, 0.05f, 0.99f };
        vertices[3] = { { sx, 0.0f, -sy }, color, 0, 0.99f, 0.99f };

        MatrixSonicModel = MatrixIdentity;
        WorldMatrixRotateY(ry);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixRotateZ(ASin(normal.z));
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixRotateX(-ASin(normal.x));
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixTranslateXYZ(x, y, z);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

        SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
        SetRenderTexture(0, ShadowTexture);
        DrawIndexedPrimitive(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
    }
}

void DrawModelSonic(float x, float y, float z, float rotation)
{
    SetRenderTexture(0, SonicTexture);
    SetRenderState(RENDER_STATE_SPECULARENABLE, true);
    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    if (SonicAni.field_BFAA == 4) {
        if (SonicAni.field_BFAC) {
            WorldMatrixRotateY(rotation);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            DrawIndexedPrimitive(RENDER_FVF_VERTEX, BallModel.vertices, BallModel.numVertices, BallModel.indices, BallModel.numIndices);
        }
        else {
            WorldMatrixTranslateXYZ(0.0f, -5.4f, 0.0f);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            WorldMatrixRotateZ(PlayerJumpRotationX);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            WorldMatrixTranslateXYZ(0.0f, 3.8f, 0.0f);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            WorldMatrixRotateY(rotation);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            DrawIndexedPrimitive(RENDER_FVF_VERTEX, SonicModel.vertices, SonicModel.numVertices, SonicModel.indices, SonicModel.numIndices);

            ObjectMaterial.diffuse.a = 0.25f;
            SetRenderMaterial(&ObjectMaterial);

            memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

            WorldMatrixSetRotateY(rotation);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            DrawIndexedPrimitive(RENDER_FVF_VERTEX, BallModel.vertices, BallModel.numVertices, BallModel.indices, BallModel.numIndices);

            ObjectMaterial.diffuse.a = 1.0f;
            SetRenderMaterial(&ObjectMaterial);
        }
    }
    else {
        WorldMatrixRotateY(rotation);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

        WorldMatrixRotateZ(PlayerRotationZ);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

        WorldMatrixRotateX(PlayerRotationX);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

        WorldMatrixTranslateXYZ(x, y, z);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

        SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
        DrawIndexedPrimitive(RENDER_FVF_VERTEX, SonicModel.vertices, SonicModel.numVertices, SonicModel.indices, SonicModel.numIndices);
    }

    SetRenderState(RENDER_STATE_SPECULARENABLE, false);
    DrawModelShadow(x, y, z, -30.0f, 2.5f, 3.0f, rotation);
}

void DrawObjectModelID(int object, float x, float y, float z, float ry, float rx, float rz)
{
    LVertex vertices[4];
    MEM_ZERO(&vertices, sizeof(vertices));

    ushort indices[6] = { 0, 1, 2, 1, 3, 2 };

    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    WorldMatrixRotateY(ry);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixRotateZ(rx);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixRotateX(rz);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixTranslateXYZ(x, y, z);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    switch (object) {
        case MODEL_ID_RING: {
            TMF *model = &ObjectModel[object];

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            SetRenderTexture(0, ObjectTexture[object]);
            SetRenderTextureStageState(0, TEXTURE_STATE_TEXCOORDINDEX, TEXTURE_VALUE_CAMERASPACE_REFLECTIONVECTOR);
            DrawIndexedPrimitive(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);
            SetRenderTextureStageState(0, TEXTURE_STATE_TEXCOORDINDEX, TEXTURE_VALUE_PASSTHRU);
            break;
        }

        case MODEL_ID_RING_SPARKLE: {
            vertices[0] = { { -2.0f, 2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.05f };
            vertices[1] = { { 2.0f, 2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.05f };
            vertices[2] = { { -2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.99f };
            vertices[3] = { { 2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.99f };

            memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

            WorldMatrixRotateX(rz);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            MatrixMultiply(&MatrixSonicModel, &MatrixInversed);

            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            SetRenderTexture(0, SparkleTexture);
            SetRenderState(RENDER_STATE_LIGHTING, 0);
            DrawIndexedPrimitive(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
            SetRenderState(RENDER_STATE_LIGHTING, 1);
            break;
        }

        default: {
            TMF *model = &ObjectModel[object];

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            SetRenderTexture(0, ObjectTexture[object]);
            DrawIndexedPrimitive(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);
            break;
        }
    }
}

void PlayerRotationPhysics(float *x, float *y, float *z)
{
    float sx = *x;
    float sy = *y;
    float sz = *z;

    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    WorldMatrixRotateZ(PlayerTargetRotationZ);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixRotateX(PlayerTargetRotationX);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    *x = MatrixSonicModel.m[0][0] * sx + MatrixSonicModel.m[1][0] * sy + MatrixSonicModel.m[2][0] * sz + MatrixSonicModel.m[3][0];
    *y = MatrixSonicModel.m[0][1] * sx + MatrixSonicModel.m[1][1] * sy + MatrixSonicModel.m[2][1] * sz + MatrixSonicModel.m[3][1];
    *z = MatrixSonicModel.m[0][2] * sx + MatrixSonicModel.m[1][2] * sy + MatrixSonicModel.m[2][2] * sz + MatrixSonicModel.m[3][2];
}

void ResetPlayerRotation()
{
    PlayerTargetRotationZ = 0.0f;
    PlayerTargetRotationX = 0.0f;
}

// Seems to move the world camera
void Matrix_40398C(Matrix3D *matrix, Vector3D &param1, Vector3D &param2, Vector3D &param3)
{
    Vector3D variable1 = param2 - param1;

    float variable2 = variable1.Magnitude();
    variable1 /= variable2;

    float variable3    = param3 * variable1;
    Vector3D variable4 = param3 - (variable3 * variable1);

    if (variable2 < 0.000001f) {
        variable4 = { 0.0f, 1.0f, 0.0f };
        variable4 = variable4 - (variable1.y * variable1);

        if (variable2 < 0.000001f) {
            variable4 = { 0.0f, 0.0f, 1.0f };
            variable4 = variable4 - (variable1.z * variable1);
        }
    }

    variable4 /= variable4.Magnitude();
    Vector3D variable5 = CrossProduct(variable4, variable1);

    IdentityMatrix(matrix);

    matrix->m[0][0] = variable5.x;
    matrix->m[0][1] = variable4.x;
    matrix->m[0][2] = variable1.x;

    matrix->m[1][0] = variable5.y;
    matrix->m[1][1] = variable4.y;
    matrix->m[1][2] = variable1.y;

    matrix->m[2][0] = variable5.z;
    matrix->m[2][1] = variable4.z;
    matrix->m[2][2] = variable1.z;

    MatrixInverse(&MatrixInversed, matrix);

    matrix->m[3][0] = param1 * variable5;
    matrix->m[3][1] = param1 * variable4;
    matrix->m[3][2] = param1 * variable1;

    matrix->m[3][0] = -matrix->m[3][0];
    matrix->m[3][1] = -matrix->m[3][1];
    matrix->m[3][2] = -matrix->m[3][2];
}

void WorldMatrixTranslateXYZ(float x, float y, float z)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[3][0] = x;
    MatrixWorld.m[3][1] = y;
    MatrixWorld.m[3][2] = z;
    MatrixWorld.m[3][3] = 1.0f;
}

void WorldMatrixRotateX(float value)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[0][0] = cos(value);
    MatrixWorld.m[0][1] = sin(value);
    MatrixWorld.m[1][0] = -sin(value);
    MatrixWorld.m[1][1] = cos(value);
}

void WorldMatrixRotateY(float value)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[0][0] = cos(value);
    MatrixWorld.m[0][2] = -sin(value);
    MatrixWorld.m[2][0] = sin(value);
    MatrixWorld.m[2][2] = cos(value);
}

void WorldMatrixRotateZ(float x)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[1][1] = cos(x);
    MatrixWorld.m[1][2] = sin(x);
    MatrixWorld.m[2][1] = -sin(x);
    MatrixWorld.m[2][2] = cos(x);
}

void WorldMatrixSetTranslateXYZ(float x, float y, float z)
{
    WorldMatrixTranslateXYZ(x, y, z);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void WorldMatrixSetRotateX(float z)
{
    WorldMatrixRotateX(z);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void WorldMatrixSetRotateY(float y)
{
    WorldMatrixRotateY(y);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void WorldMatrixSetRotateZ(float x)
{
    WorldMatrixRotateZ(x);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}