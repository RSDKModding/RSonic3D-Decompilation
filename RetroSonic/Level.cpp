#include "RetroEngine.hpp"

InputData MGameInput;

int GameMode;
int MainGameMode;

int Debug;
int DebugEn = true;

byte PauseV;
byte TimeCount;
byte SmallSeconds;
byte Seconds;
byte Minutes;

float RingRotationY;

Vector3D UnusedVector;
Vector3D CameraEye;
Vector3D CameraPosition;
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

Animator SonicAni;
Material RenderMaterial;

// i'm not sure what these are for
// these are set but never read
Matrix3D SonicNodeMatrixPositionStore[100][36];
Matrix3D SonicNodeMatrixNormalStore[100][36];

void LoadObjectAssets()
{
    LoadModel(&LogoModel, "Data/Title/Logo.tmf");
    LoadModel(&BGModel, "Data/Levels/TestZone/BG.tmf");
    LoadModel(&SonicBaseModel, "Data/Characters/Sonic.tmf");
    LoadModel(&SonicModel, "Data/Characters/Sonic.tmf");
    LoadModel(&BallModel, "Data/Characters/Ball.tmf");
    LoadAnimationFile(&SonicAni, "Data/Characters/Sonic.ani");
    ProcessPlayerAnimationLMC();

    LoadModel(&ObjectModel[0], "Data/Objects/General/Ring.tmf");
    LoadTexture(&ObjectTexture[0], "Data/Objects/General/Ring.png", false);

    LoadModel(&ObjectModel[1], "Data/Objects/General/Spring.tmf");
    LoadTexture(&ObjectTexture[1], "Data/Objects/General/Spring.png", false);

    LoadTexture(&SparkleTexture, "Data/Objects/General/Sparkle.png", false);

    Animator *animator = &SonicAni;

    animator->animationID   = ANI_TITLESCREEN;
    animator->nextAnimation = ANI_TITLESCREEN;
    animator->frameID       = 0;
    animator->nextFrame     = 0;
    animator->frameTimer    = 0;
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

void DrawMainGameGfx()
{
    BeginScene();
    ClearScreen(0x0000BE);
    SetSceneRenderProperties(1);

    PlayerObject *player  = &Player[PNumber];
    PlayerObject *player1 = &Player[0];

    float x = player1->position.x;
    float y = player1->position.y;
    float z = player1->position.z;

    UpdateCamera(x, y - player1->collisionPos.y, z, CameraPosition.x, CameraPosition.y, CameraPosition.z, CameraRotateY);
    DrawLevelModel();

    if (Debug) {
        DrawObjectModel(OBJ_MODEL_RING, x, y + 4.0f, z, x * 0.1f, 0.0f, 0.0f);
        DrawModelShadow(x, y + 4.0f, z, -30.0f, 2.0f, 1.0f, x * 0.1f);
    }
    else {
        DrawModelSonic(x, y, z, -player->rotationY + RSDK_PI);
    }

    DrawObjects();
    EndScene();
}

void DrawLevelModel()
{
    LMF *level = &LevelModel;

    float c = (CameraCullX - level->x) / 50.0f;
    float r = (CameraCullZ - level->z) / 50.0f;

    SetRenderMaterial(&RenderMaterial);
    SetRenderState(RENDER_STATE_LIGHTING, false);
    SetRenderState(RENDER_STATE_ZENABLE, false);
    SetRenderTexture(0, BGTexture);

    MatrixWorldSetTranslateXYZ(CameraEye.x, CameraEye.y, CameraEye.z);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
    DrawFace(RENDER_FVF_VERTEX, BGModel.vertices, BGModel.numVertices, BGModel.indices, BGModel.numIndices);

    SetRenderState(RENDER_STATE_ZENABLE, true);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixIdentity);
    SetRenderTexture(0, LevelTexture[level->textureIDs[0]]);

    byte texture = level->textureIDs[0];
    for (int l = 0; l < level->layers; ++l) {
        if (texture != level->textureIDs[l]) {
            texture = level->textureIDs[l];
            SetRenderTexture(0, LevelTexture[texture]);
        }

        for (int y = CLAMP(r - 12, 0, level->rows); y < CLAMP(r + 12, 0, level->rows); ++y) {
            for (int x = CLAMP(c - 12, 0, level->columns); x < CLAMP(c + 12, 0, level->columns); ++x) {
                LMFMesh *tile = &level->tiles[l][y][x];
                if (tile->numVertices) {
                    for (int v = 0; v < tile->numVertices; ++v) {
                        LVertex *vert = &tile->vertices[v];

                        Vector3D z;
                        z.x = vert->x - CameraPosition.x;
                        z.y = vert->y - CameraPosition.y;
                        z.z = vert->z - CameraPosition.z;

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

                    DrawFace(RENDER_FVF_LVERTEX, tile->vertices, tile->numVertices, tile->indices, tile->numIndices);
                }
            }
        }
    }

    SetRenderState(RENDER_STATE_LIGHTING, true);
}

void UpdateCamera(float x, float y, float z, float cx, float cy, float cz, float ry)
{
    Matrix3D *matrix = &MatrixObject;
    memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

    // cheating a little with this function's name, this is also responsible for handling player rotations
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

    CameraEye.x = matrix->m[0][0] * cx + matrix->m[1][0] * cy + matrix->m[2][0] * cz + matrix->m[3][0];
    CameraEye.y = matrix->m[0][1] * cx + matrix->m[1][1] * cy + matrix->m[2][1] * cz + matrix->m[3][1];
    CameraEye.z = matrix->m[0][2] * cx + matrix->m[1][2] * cy + matrix->m[2][2] * cz + matrix->m[3][2];

    memcpy(matrix, &MatrixIdentity, sizeof(*matrix));

    Vector3D rotation;
    rotation.x = matrix->m[0][0] * 0.0f + matrix->m[1][0] * 1.0f + matrix->m[2][0] * 0.0f + matrix->m[3][0];
    rotation.y = matrix->m[0][1] * 0.0f + matrix->m[1][1] * 1.0f + matrix->m[2][1] * 0.0f + matrix->m[3][1];
    rotation.z = matrix->m[0][2] * 0.0f + matrix->m[1][2] * 1.0f + matrix->m[2][2] * 0.0f + matrix->m[3][2];

    CameraCullX = cx - Sin(ry) * 500.0f;
    CameraCullZ = cz + Cos(ry) * 500.0f;

    Vector3D eye = { CameraEye };
    Vector3D xyz = { x, y, z };
    CameraLook(&MatrixView, eye, xyz, rotation);
    SetRenderTransform(RENDER_TRANSFORM_VIEW, &MatrixView);
}

void CameraLook(Matrix3D *matrix, Vector3D &eye, Vector3D &target, Vector3D &rotation)
{
    Vector3D forward = target - eye;
    forward /= forward.Magnitude();

    float angle = rotation * forward;
    Vector3D up = rotation - (angle * forward);

    if (up.Magnitude() < epsilon) {
        up = Vector3D(0.0f, 1.0f, 0.0f) - (forward.y * forward);
        if (up.Magnitude() < epsilon)
            up = Vector3D(0.0f, 0.0f, 1.0f) - (forward.z * forward);
    }

    up /= up.Magnitude();
    Vector3D right = CrossProduct(up, forward);

    IdentityMatrix(matrix);

    matrix->m[0][0] = right.x;
    matrix->m[0][1] = up.x;
    matrix->m[0][2] = forward.x;

    matrix->m[1][0] = right.y;
    matrix->m[1][1] = up.y;
    matrix->m[1][2] = forward.y;

    matrix->m[2][0] = right.z;
    matrix->m[2][1] = up.z;
    matrix->m[2][2] = forward.z;

    MatrixInverse(&MatrixInversed, matrix);

    matrix->m[3][0] = eye * right;
    matrix->m[3][1] = eye * up;
    matrix->m[3][2] = eye * forward;

    matrix->m[3][0] = -matrix->m[3][0];
    matrix->m[3][1] = -matrix->m[3][1];
    matrix->m[3][2] = -matrix->m[3][2];
}