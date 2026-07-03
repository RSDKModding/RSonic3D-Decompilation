#include "RetroEngine.hpp"

Matrix3D MatrixSonicModel;
Matrix3D MatrixSonicNodeRotation[36];
Matrix3D MatrixSonicNodeTransform[36];

float PlayerTargetRotationZ;
float PlayerTargetRotationX;

float PlayerRotationZ;
float PlayerRotationX;

float PlayerPrevRotationZ;
float PlayerPrevRotationX;

int PlayerRotationTimerX;
int PlayerRotationTimerZ;

float data_4C9D4C;

byte PNumber;
PlayerObject Player[2];

void ProcessPlayerCamera()
{
    int v0;    // esi
    int v1;    // esi
    int v2;    // esi
    int v3;    // esi
    float v4;  // [esp+0h] [ebp-28h]
    float v5;  // [esp+8h] [ebp-20h]
    float v6;  // [esp+Ch] [ebp-1Ch]
    float v7;  // [esp+10h] [ebp-18h]
    float v8;  // [esp+14h] [ebp-14h]
    float v9;  // [esp+18h] [ebp-10h]
    float v10; // [esp+1Ch] [ebp-Ch]
    float v11; // [esp+1Ch] [ebp-Ch]
    float v12; // [esp+20h] [ebp-8h]
    float v13; // [esp+20h] [ebp-8h]
    float v14; // [esp+24h] [ebp-4h]

    if (MGameInput.Z == true) {
        data_4C9F68.x = data_4C9F68.x - Player[PNumber].position.x;
        data_4C9F68.z = data_4C9F68.z - Player[PNumber].position.z;
        v9            = Cos(0.02) * data_4C9F68.x;
        v12           = Sin(0.02) * data_4C9F68.z + v9;
        v8            = -Sin(0.02) * data_4C9F68.x;
        v10           = Cos(0.02) * data_4C9F68.z + v8;
        data_4C9F68.x = v12 + Player[PNumber].position.x;
        data_4C9F68.z = v10 + Player[PNumber].position.z;
    }

    if (MGameInput.X == true) {
        data_4C9F68.x = data_4C9F68.x - Player[PNumber].position.x;
        data_4C9F68.z = data_4C9F68.z - Player[PNumber].position.z;
        v7            = Cos(-0.02) * data_4C9F68.x;
        v13           = Sin(-0.02) * data_4C9F68.z + v7;
        v6            = -Sin(-0.02) * data_4C9F68.x;
        v11           = Cos(-0.02) * data_4C9F68.z + v6;
        data_4C9F68.x = v13 + Player[PNumber].position.x;
        data_4C9F68.z = v11 + Player[PNumber].position.z;
    }

    if (data_4C9F68.x == Player[PNumber].position.x) {
        if (data_4C9F68.z >= (double)Player[PNumber].position.z)
            data_4C9F74 = 3.1415927;
        else
            data_4C9F74 = 0.0;
    }
    else {
        v4 = (data_4C9F68.z - Player[PNumber].position.z) / (data_4C9F68.x - Player[PNumber].position.x);
        if (data_4C9F68.x <= (double)Player[PNumber].position.x)
            data_4C9F74 = ATan(v4) - 3.1415927 * 0.5;
        else
            data_4C9F74 = ATan(v4) + 3.1415927 * 0.5;
    }

    v5  = Player[PNumber].position.x - data_4C9F68.x;
    v14 = v5 / Sin(data_4C9F74);
    if (v14 < 0.0)
        v14 = v14 * -1.0;

    if (v14 < 32.0f) {
        data_4C9F68.x = Player[PNumber].position.x - Sin(data_4C9F74) * -32.0;
        data_4C9F68.z = Cos(data_4C9F74) * -32.0 + Player[PNumber].position.z;
    }

    if (v14 > 60.0f) {
        data_4C9F68.x = Player[PNumber].position.x - Sin(data_4C9F74) * -60.0;
        data_4C9F68.z = Cos(data_4C9F74) * -60.0 + Player[PNumber].position.z;
    }
}

void LoadPlayerGfx(const char *textureName, sbyte characterID)
{
#if RETRO_USE_ORIGINAL_CODE
    char name[64];
    lstrcpy(name, "Data/Characters/");
    lstrcat(name, textureName);

    if (CharacterTexture[characterID] != NULL)
        CharacterTexture[characterID]->Release();

    CharacterTexture[characterID] = (Texture *)(CreateTexture(D3DDevice, name));

    if (characterID == CHARACTER_SONIC_TAILS)
        DDLoadBitmap(name, 0);
#endif
}

void HandleSonicVertexPositions(int frameID)
{
    AnimationNode *node = &SonicAni.nodes[frameID];

    Matrix3D *matrix = &MatrixSonicModel;
    for (int i = 0; i < node->vertexCount; ++i) {
        Vertex *vert = &SonicModel.vertices[node->vertexIDs[i]];
        Vertex *base = &SonicBaseModel.vertices[node->vertexIDs[i]];

        vert->x = matrix->m[0][0] * base->x + matrix->m[1][0] * base->y + matrix->m[2][0] * base->z + matrix->m[3][0];
        vert->y = matrix->m[0][1] * base->x + matrix->m[1][1] * base->y + matrix->m[2][1] * base->z + matrix->m[3][1];
        vert->z = matrix->m[0][2] * base->x + matrix->m[1][2] * base->y + matrix->m[2][2] * base->z + matrix->m[3][2];
    }
}

void HandleSonicVertexNormals(int frameID)
{
    AnimationNode *node = &SonicAni.nodes[frameID];

    Matrix3D *matrix = &MatrixSonicModel;
    for (int i = 0; i < node->vertexCount; ++i) {
        Vertex *vert = &SonicModel.vertices[node->vertexIDs[i]];
        Vertex *base = &SonicBaseModel.vertices[node->vertexIDs[i]];

        vert->nx = matrix->m[0][0] * base->nx + matrix->m[1][0] * base->ny + matrix->m[2][0] * base->nz + matrix->m[3][0];
        vert->ny = matrix->m[0][1] * base->nx + matrix->m[1][1] * base->ny + matrix->m[2][1] * base->nz + matrix->m[3][1];
        vert->nz = matrix->m[0][2] * base->nx + matrix->m[1][2] * base->ny + matrix->m[2][2] * base->nz + matrix->m[3][2];
    }
}

void SonicModel_405CE2(uint8_t a1, float a2)
{
    if (a1 != SonicAni.field_BFAB) {
        SonicAni.field_BFAB = a1;
        SonicAni.field_BFB0 = 0;
        SonicAni.field_BFAE = 0;
        if (SonicAni.field_BFAA == 4) {
            SonicAni.field_BFAA = SonicAni.field_BFAB;
            SonicAni.field_BFAC = 0;
            data_4C9D4C         = 0.0f;
        }
    }

    if (a1 == 2) {
        SonicAni.states[2].frameDuration = (long)(a2 * 128.0);
    }
    else if (a1 == 4) {
        data_4C9D4C = data_4C9D4C - a2 * 0.30000001f;
        if (data_4C9D4C < 0.0f)
            data_4C9D4C = 3.1415927f + 3.1415927f;
        SonicAni.states[4].frameDuration = (long)(a2 * 128.0);
    }
}

void ProcessPlayerAnimationLMC()
{
    int v0; // edx

    bool v7 = false;
    if (SonicAni.field_BFAA == SonicAni.field_BFAB)
        v0 = SonicAni.field_BFAA;
    else
        v0 = SonicAni.field_BFAB;

    SonicAni.field_BFB0 += SonicAni.states[v0].frameDuration;
    if (SonicAni.field_BFB0 >= 240) {
        SonicAni.field_BFB0 -= 240;

        if (++SonicAni.field_BFAC >= (int)SonicAni.states[SonicAni.field_BFAA].frameCount)
            SonicAni.field_BFAC = SonicAni.states[SonicAni.field_BFAA].loopIndex;

        if (SonicAni.field_BFAA != SonicAni.field_BFAB) {
            SonicAni.field_BFAA = SonicAni.field_BFAB;
            SonicAni.field_BFAC = 0;
        }

        SonicAni.field_BFAE = SonicAni.field_BFAC + 1;
        if (SonicAni.field_BFAE >= (int)SonicAni.states[SonicAni.field_BFAA].frameCount)
            SonicAni.field_BFAE = SonicAni.states[SonicAni.field_BFAA].loopIndex;
    }

    float F_BFB0 = (double)SonicAni.field_BFB0 / 240.0;
    for (int i = 0; i < 36; ++i) {
        AnimationNode *node = &SonicAni.nodes[i];

        AnimationState *AB90_BFAA = &SonicAni.states[SonicAni.field_BFAA];
        AnimationState *AB90_BFAB = &SonicAni.states[SonicAni.field_BFAB];

        ushort BFAA_BFAC = AB90_BFAA->array_2[SonicAni.field_BFAC];
        ushort BFAB_BFAE = AB90_BFAB->array_2[SonicAni.field_BFAE];

        memcpy(&MatrixSonicNodeRotation[i], &MatrixIdentity, sizeof(Matrix3D));
        WorldMatrixRotateZ((1.0 - F_BFB0) * node->rotX[BFAA_BFAC] + F_BFB0 * node->rotX[BFAB_BFAE]);
        MatrixMultiply(&MatrixSonicNodeRotation[i], &MatrixWorld);

        WorldMatrixRotateY((1.0 - F_BFB0) * node->rotY[BFAA_BFAC] + F_BFB0 * node->rotY[BFAB_BFAE]);
        MatrixMultiply(&MatrixSonicNodeRotation[i], &MatrixWorld);

        WorldMatrixRotateX((1.0 - F_BFB0) * node->rotZ[BFAA_BFAC] + F_BFB0 * node->rotZ[BFAB_BFAE]);
        MatrixMultiply(&MatrixSonicNodeRotation[i], &MatrixWorld);

        memcpy(&MatrixSonicNodeTransform[i], &MatrixIdentity, sizeof(Matrix3D));
        WorldMatrixTranslateXYZ(-node->position.x, -node->position.y, -node->position.z);
        MatrixMultiply(&MatrixSonicNodeTransform[i], &MatrixWorld);

        MatrixMultiply(&MatrixSonicNodeTransform[i], &MatrixSonicNodeRotation[i]);
        WorldMatrixTranslateXYZ(node->position.x, node->position.y, node->position.z);
        MatrixMultiply(&MatrixSonicNodeTransform[i], &MatrixWorld);
    }

    for (int j = 0; j < SonicAni.frameIDCount; ++j) {
        if (v7 != false) {
            switch (SonicAni.frameIDs[j]) {
                case 254: v7 = true; break;
                case 255: v7 = false; break;

                default:
                    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                    for (int k = j; SonicAni.frameIDs[k] < 254; ++k) {
                        MatrixMultiply(&MatrixSonicModel, &MatrixSonicNodeTransform[SonicAni.frameIDs[k]]);
                    }
                    HandleSonicVertexPositions(SonicAni.frameIDs[j]);

                    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                    for (int l = j; SonicAni.frameIDs[l] < 254; ++l) {
                        MatrixMultiply(&MatrixSonicModel, &MatrixSonicNodeRotation[SonicAni.frameIDs[l]]);
                    }
                    HandleSonicVertexNormals(SonicAni.frameIDs[j]);
                    break;
            }
        }
        else {
            switch (SonicAni.frameIDs[j]) {
                case 254: v7 = true; break;
                case 255: v7 = false; break;

                default:
                    memcpy(&MatrixSonicModel, &MatrixSonicNodeTransform[SonicAni.frameIDs[j]], sizeof(MatrixSonicModel));
                    HandleSonicVertexPositions(SonicAni.frameIDs[j]);

                    memcpy(&MatrixSonicModel, &MatrixSonicNodeRotation[SonicAni.frameIDs[j]], sizeof(MatrixSonicModel));
                    HandleSonicVertexNormals(SonicAni.frameIDs[j]);
                    break;
            }
        }
    }
}

void MightBeSonicAnim_406432()
{
    bool v6 = false;
    for (int i = 0; i < 36; ++i) {
        for (int j = 0; j < 36; ++j) {
            AnimationNode *node = &SonicAni.nodes[j];

            memcpy(&MatrixSonicNodeRotation[j], &MatrixIdentity, sizeof(Matrix3D));
            WorldMatrixRotateZ(node->rotX[i]);

            MatrixMultiply(&MatrixSonicNodeRotation[j], &MatrixWorld);
            WorldMatrixRotateY(node->rotY[i]);

            MatrixMultiply(&MatrixSonicNodeRotation[j], &MatrixWorld);
            WorldMatrixRotateX(node->rotZ[i]);

            MatrixMultiply(&MatrixSonicNodeRotation[j], &MatrixWorld);
            memcpy(&MatrixSonicNodeTransform[j], &MatrixIdentity, sizeof(Matrix3D));

            WorldMatrixTranslateXYZ(-node->position.x, -node->position.y, -node->position.z);
            MatrixMultiply(&MatrixSonicNodeTransform[j], &MatrixWorld);
            MatrixMultiply(&MatrixSonicNodeTransform[j], &MatrixSonicNodeRotation[j]);
            WorldMatrixTranslateXYZ(node->position.x, node->position.y, node->position.z);
            MatrixMultiply(&MatrixSonicNodeTransform[j], &MatrixWorld);
        }

        for (int k = 0; k < SonicAni.frameIDCount; ++k) {
            if (v6 != false) {
                switch (SonicAni.frameIDs[k]) {
                    case 254: v6 = true; break;
                    case 255: v6 = false; break;

                    default:
                        memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                        for (int m = k; SonicAni.frameIDs[m] < 254; ++m) {
                            MatrixMultiply(&MatrixSonicModel, &MatrixSonicNodeTransform[SonicAni.frameIDs[m]]);
                        }
                        memcpy(&matrix_47A790[k][i], &MatrixSonicModel, sizeof(matrix_47A790[k][i]));

                        memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                        for (int n = k; SonicAni.frameIDs[n] < 254; ++n) {
                            MatrixMultiply(&MatrixSonicModel, &MatrixSonicNodeRotation[SonicAni.frameIDs[n]]);
                        }
                        memcpy(&array_42C590[k][i], &MatrixSonicModel, sizeof(array_42C590[k][i]));
                        break;
                }
            }
            else {
                switch (SonicAni.frameIDs[k]) {
                    case 254: v6 = true; break;
                    case 255: v6 = false; break;

                    default:
                        memcpy(&MatrixSonicModel, &MatrixSonicNodeTransform[SonicAni.frameIDs[k]], sizeof(MatrixSonicModel));
                        HandleSonicVertexPositions(SonicAni.frameIDs[k]);

                        memcpy(&MatrixSonicModel, &MatrixSonicNodeRotation[SonicAni.frameIDs[k]], sizeof(MatrixSonicModel));
                        HandleSonicVertexNormals(SonicAni.frameIDs[k]);
                        break;
                }
            }
        }
    }
}