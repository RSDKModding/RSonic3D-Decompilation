#include "RetroEngine.hpp"

float PlayerTargetRotationZ;
float PlayerTargetRotationX;

float PlayerRotationZ;
float PlayerRotationX;

float PlayerPrevRotationZ;
float PlayerPrevRotationX;

int PlayerRotationTimerX;
int PlayerRotationTimerZ;

byte PNumber;
PlayerObject Player[2];

void ProcessPlayerInput()
{
    float a4;  // [esp+0h] [ebp-138h]
    float a4a; // [esp+0h] [ebp-138h]
    float v2;  // [esp+4h] [ebp-134h]
    float v3;  // [esp+4h] [ebp-134h]
    float v4;  // [esp+4h] [ebp-134h]
    float v5;  // [esp+4h] [ebp-134h]
    float v6;  // [esp+4h] [ebp-134h]
    float v7;  // [esp+4h] [ebp-134h]
    float v8;  // [esp+4h] [ebp-134h]
    float v9;  // [esp+4h] [ebp-134h]
    int v18;   // [esp+134h] [ebp-4h]

    v18                    = 0;
    Player[PNumber].f_0x48 = 0;
    Player[PNumber].f_0x4C = 0;
    if (!Player[PNumber].f_0x3B) {
        Player[PNumber].f_0x2C = 0;

        CheckInput(&MGameInput);

        if (Debug) {
            if (MGameInput.left == 1) {
                Player[0].position.x = Player[0].position.x - Cos(data_4C9F74);
                data_4C9F68.x        = data_4C9F68.x - Cos(data_4C9F74);
                Player[0].position.z = Player[0].position.z - Sin(data_4C9F74);
                data_4C9F68.z        = data_4C9F68.z - Sin(data_4C9F74);
            }
            if (MGameInput.right == 1) {
                Player[0].position.x = Cos(data_4C9F74) + Player[0].position.x;
                data_4C9F68.x        = Cos(data_4C9F74) + data_4C9F68.x;
                Player[0].position.z = Sin(data_4C9F74) + Player[0].position.z;
                data_4C9F68.z        = Sin(data_4C9F74) + data_4C9F68.z;
            }
            if (MGameInput.Z) {
                if (MGameInput.up == 1) {
                    Player[0].position.y += 1.0;
                    data_4C9F68.y += 1.2;
                }
                if (MGameInput.down == 1) {
                    Player[0].position.y -= 1.0f;
                    data_4C9F68.y -= 1.2f;
                }
            }
            else {
                if (MGameInput.up == 1) {
                    v2                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.x = Player[0].position.x - Cos(v2);
                    v3                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F68.x        = data_4C9F68.x - Cos(v3);
                    v4                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.z = Player[0].position.z - Sin(v4);
                    v5                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F68.z        = data_4C9F68.z - Sin(v5);
                }
                if (MGameInput.down == 1) {
                    v6                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.x = Cos(v6) + Player[0].position.x;
                    v7                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F68.x        = Cos(v7) + data_4C9F68.x;
                    v8                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.z = Sin(v8) + Player[0].position.z;
                    v9                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F68.z        = Sin(v9) + data_4C9F68.z;
                }
            }

            CheckKeyPress(&MGameInput, INPUT_START, INPUT_ONCE);

            if (MGameInput.control == 1) {
                a4 = Player[0].position.y + 1.8;
                CreateObject(4, 0, Player[0].position.x, a4, Player[0].position.z);
            }

            if (MGameInput.X == 1) {
                a4a = Player[0].position.y + 4.0;
                CreateObject(1, 0, Player[0].position.x, a4a, Player[0].position.z);
            }
        }
        else {
            if (MGameInput.left == 1) {
                Player[PNumber].f_0x2C += 64;
                if (Player[PNumber].f_0x28 > 192)
                    Player[PNumber].f_0x28 -= 256;
                ++v18;
            }

            if (MGameInput.right == 1) {
                if (Player[PNumber].f_0x28 < 64)
                    Player[PNumber].f_0x28 += 256;
                Player[PNumber].f_0x2C += 192;
                ++v18;
            }

            if (MGameInput.up == 1) {
                if (Player[PNumber].f_0x28 > 128)
                    Player[PNumber].f_0x2C += 256;
                ++v18;
            }

            if (MGameInput.down == 1) {
                Player[PNumber].f_0x2C += 128;
                ++v18;
            }

            if (v18 > 0) {
                Player[PNumber].f_0x48 = 1;
                Player[PNumber].f_0x2C /= v18;
            }

            Player[PNumber].f_0x50 = MGameInput.Z == 1;
            CheckKeyPress(&MGameInput, INPUT_START, INPUT_Z);
        }

        if (MGameInput.control == 1)
            Player[PNumber].f_0x4C = 1;

        if (MGameInput.shift == 1 && dword_41F480 == 1)
            Debug = Debug == 0;
    }

    CheckInput(&MGameInput);
}

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

void ProcessPlayerMovement()
{
    PlayerObject *player = &Player[PNumber];

    float a6; // [esp+0h] [ebp-14h]
    float v1; // [esp+4h] [ebp-10h]
    float v2; // [esp+4h] [ebp-10h]
    char v3;  // [esp+8h] [ebp-Ch]

    if (player->f_0x38 == 1 && player->f_0x48 == 1) {
        if (player->f_0x28 < player->f_0x2C) {
            player->f_0x28 += 6;
            if (player->f_0x24 > 0.0)
                player->f_0x24 -= 0.02;
            if (player->f_0x28 > player->f_0x2C)
                player->f_0x28 = player->f_0x2C;
        }

        if (player->f_0x28 > player->f_0x2C) {
            player->f_0x28 -= 6;
            if (player->f_0x24 > 0.0)
                player->f_0x24 -= 0.02;
            if (player->f_0x28 < player->f_0x2C)
                player->f_0x28 = player->f_0x2C;
        }

        if (player->f_0x24 < 2.4000001)
            player->f_0x24 += 0.012;
    }
    else if (player->f_0x38 == 0 && player->f_0x48 == 1) {
        if (player->f_0x28 < player->f_0x2C) {
            player->f_0x28 += 8;
            if (player->f_0x24 > 0.0)
                player->f_0x24 -= 0.0099999998;
            if (player->f_0x28 > player->f_0x2C)
                player->f_0x28 = player->f_0x2C;
        }

        if (player->f_0x28 > player->f_0x2C) {
            player->f_0x28 -= 8;
            if (player->f_0x24 > 0.0)
                player->f_0x24 -= 0.0099999998;
            if (player->f_0x28 < player->f_0x2C)
                player->f_0x28 = player->f_0x2C;
        }

        if (player->f_0x24 < 2.4000001)
            player->f_0x24 = player->f_0x24 + 0.012;
    }
    else {
        if (player->f_0x24 > 0.0)
            player->f_0x24 -= 0.012;

        if (player->f_0x24 > -0.050000001 && player->f_0x24 < 0.0)
            player->f_0x24 = 0.0;
    }

    if (player->f_0x38 != 2) {
        if (player->f_0x44 == true) {
            player->f_0x38 = 1;

            player->velocity.y -= 0.050000001;
            if (player->velocity.y > 4.0) {
                player->velocity.y = 4.0;
                ResetPlayerRotation();
            }

            v1 = player->f_0x24 * 0.40000001 + 0.40000001;
            SonicModel_405CE2(4, v1);
        }
        else {
            if (Player[0].f_0x24 < 0.0099999998) {
                SonicModel_405CE2(0, 0.0);
            }
            else {
                v2 = player->f_0x24 * 0.40000001;
                SonicModel_405CE2(2, v2);
            }

            player->f_0x38 = 0;
            if (player->f_0x24 == 0.0)
                player->gap_54[0] = 0;

            if (player->f_0x4C == 1) {
                player->f_0x44     = true;
                player->velocity.y = 2.0;
                player->f_0x38     = 1;
                ResetPlayerRotation();
            }
        }
    }

    player->velocity.x = -Sin(player->f_0x30) * player->f_0x24;
    player->velocity.z = Cos(player->f_0x30) * player->f_0x24;
    player->f_0x18.x   = 0.0;
    player->f_0x18.y   = -4.0;
    player->f_0x18.z   = 0.0;

    if (player->f_0x44 == true) {
        if (player->velocity.y < 0.0) {
            a6 = player->velocity.y * -1.0;
            if (ObjectFloorCollision(&player->position.x, &player->position.y, &player->position.z, player->velocity.x, a6, player->velocity.z)
                == 1) {
                player->velocity.y = 0.0;
                player->f_0x44     = false;
                player->f_0x38     = 0;
            }
        }
    }
    else {
        player->velocity.y = 0.0;
        PlayerRotationPhysics(&player->velocity.x, &player->velocity.y, &player->velocity.z);
        player->f_0x18.x = 0.0;
        player->f_0x18.y = -3.5;
        player->f_0x18.z = 0.0;
        PlayerRotationPhysics(&player->f_0x18.x, &player->f_0x18.y, &player->f_0x18.z);
        player->position.x = player->position.x - (player->f_0x18.x - player->f_0x18.x * 0.25);
        player->position.y = player->position.y - (player->f_0x18.y - player->f_0x18.y * 0.25);
        player->position.z = player->position.z - (player->f_0x18.z - player->f_0x18.z * 0.25);

        v3 =
            ObjectFloorCollision(&player->position.x, &player->position.y, &player->position.z, player->f_0x18.x, player->f_0x18.y, player->f_0x18.z);
        if (v3) {
            if (v3 > 0 && v3 <= 2) {
                player->f_0x44 = false;
                player->f_0x38 = 0;
            }
        }
        else {
            player->position.x = player->f_0x18.x - player->f_0x18.x * 0.25 + player->position.x;
            player->position.y = player->f_0x18.y - player->f_0x18.y * 0.25 + player->position.y;
            player->position.z = player->f_0x18.z - player->f_0x18.z * 0.25 + player->position.z;
            player->f_0x44     = true;
        }
    }

    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y;
    player->position.z += player->velocity.z;
    ProcessPlayerCamera();

    if (player->f_0x48 == 1)
        player->f_0x30 = (double)player->f_0x28 / 128.0 * 3.1415927 + data_4C9F74;

    if (player->f_0x44 == true) {
        if (data_4C9F7C < 30)
            data_4C9F7C++;

        if (data_4C9F78 < (double)player->velocity.y) {
            data_4C9F78 += 0.1;
            if (data_4C9F78 > (double)player->velocity.y)
                data_4C9F78 = player->velocity.y;
        }

        if (data_4C9F78 > (double)player->velocity.y) {
            data_4C9F78 -= 0.1;
            if (data_4C9F78 < (double)player->velocity.y)
                data_4C9F78 = player->velocity.y;
        }
    }
    else {
        if (data_4C9F7C > 0)
            data_4C9F7C--;

        if (data_4C9F78 > 0.0) {
            data_4C9F78 -= 0.1;
            if (data_4C9F78 < 0.0)
                data_4C9F78 = 0.0;
        }

        if (data_4C9F78 < 0.0) {
            data_4C9F78 += 0.1;
            if (data_4C9F78 > 0.0)
                data_4C9F78 = 0.0;
        }
    }

    data_4C9F68.y = player->position.y + 15.0 - (double)data_4C9F7C * 0.25 * data_4C9F78;
}

void LoadPlayerGfx(const char *textureName, sbyte playerID)
{
    using namespace RenderDevice; // temp

    char name[64];
    lstrcpy(name, "Data/Characters/");
    lstrcat(name, textureName);

    if (surfaceCharacters[playerID] != NULL)
        surfaceCharacters[playerID]->Release();

    surfaceCharacters[playerID] = CreateTexture(D3DDevice, name);

    if (playerID == 0)
        DDLoadBitmap(name, 0);
}

void HandleSonicVertexPositions(int frameID)
{
    AnimationNode *node = &SonicAni.nodes[frameID];

    for (int i = 0; i < node->vertexCount; ++i) {
        D3DVERTEX *vert = &SonicMdl.vertices[node->vertexIDs[i]];
        D3DVERTEX *base = &SonicBaseMdl.vertices[node->vertexIDs[i]];

        vert->x =
            MatrixSonicModel.m[0][0] * base->x + MatrixSonicModel.m[1][0] * base->y + MatrixSonicModel.m[2][0] * base->z + MatrixSonicModel.m[3][0];
        vert->y =
            MatrixSonicModel.m[0][1] * base->x + MatrixSonicModel.m[1][1] * base->y + MatrixSonicModel.m[2][1] * base->z + MatrixSonicModel.m[3][1];
        vert->z =
            MatrixSonicModel.m[0][2] * base->x + MatrixSonicModel.m[1][2] * base->y + MatrixSonicModel.m[2][2] * base->z + MatrixSonicModel.m[3][2];
    }
}

void HandleSonicVertexNormals(int frameID)
{
    AnimationNode *node = &SonicAni.nodes[frameID];

    for (int i = 0; i < node->vertexCount; ++i) {
        D3DVERTEX *vert = &SonicMdl.vertices[node->vertexIDs[i]];
        D3DVERTEX *base = &SonicBaseMdl.vertices[node->vertexIDs[i]];

        vert->nx = MatrixSonicModel.m[0][0] * base->nx + MatrixSonicModel.m[1][0] * base->ny + MatrixSonicModel.m[2][0] * base->nz
                   + MatrixSonicModel.m[3][0];
        vert->ny = MatrixSonicModel.m[0][1] * base->nx + MatrixSonicModel.m[1][1] * base->ny + MatrixSonicModel.m[2][1] * base->nz
                   + MatrixSonicModel.m[3][1];
        vert->nz = MatrixSonicModel.m[0][2] * base->nx + MatrixSonicModel.m[1][2] * base->ny + MatrixSonicModel.m[2][2] * base->nz
                   + MatrixSonicModel.m[3][2];
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
        SonicAni.states[2].frameDuration = (__int64)(a2 * 128.0);
    }
    else if (a1 == 4) {
        data_4C9D4C = data_4C9D4C - a2 * 0.30000001f;
        if (data_4C9D4C < 0.0f)
            data_4C9D4C = 3.1415927f + 3.1415927f;
        SonicAni.states[4].frameDuration = (__int64)(a2 * 128.0);
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

        memcpy(&MatrixSonicAni_4C8990[i], &MatrixIdentity, sizeof(D3DMATRIX));

        WorldMatrixRotateZ((1.0 - F_BFB0) * node->rotX[BFAA_BFAC] + F_BFB0 * node->rotX[BFAB_BFAE]);
        MatrixMultiply(&MatrixSonicAni_4C8990[i], &MatrixWorld);

        WorldMatrixRotateY((1.0 - F_BFB0) * node->rotY[BFAA_BFAC] + F_BFB0 * node->rotY[BFAB_BFAE]);
        MatrixMultiply(&MatrixSonicAni_4C8990[i], &MatrixWorld);

        WorldMatrixRotateX((1.0 - F_BFB0) * node->rotZ[BFAA_BFAC] + F_BFB0 * node->rotZ[BFAB_BFAE]);
        MatrixMultiply(&MatrixSonicAni_4C8990[i], &MatrixWorld);

        memcpy(&MatrixSonicAni_4C9290[i], &MatrixIdentity, sizeof(D3DMATRIX));
        WorldMatrixTranslateXYZ(-node->position.x, -node->position.y, -node->position.z);
        MatrixMultiply(&MatrixSonicAni_4C9290[i], &MatrixWorld);

        MatrixMultiply(&MatrixSonicAni_4C9290[i], &MatrixSonicAni_4C8990[i]);
        WorldMatrixTranslateXYZ(node->position.x, node->position.y, node->position.z);
        MatrixMultiply(&MatrixSonicAni_4C9290[i], &MatrixWorld);
    }

    for (int j = 0; j < SonicAni.frameIDCount; ++j) {
        if (v7 != false) {
            switch (SonicAni.frameIDs[j]) {
                case 254: v7 = true; break;
                case 255: v7 = false; break;

                default:
                    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                    for (int k = j; SonicAni.frameIDs[k] < 254; ++k) {
                        MatrixMultiply(&MatrixSonicModel, &MatrixSonicAni_4C9290[SonicAni.frameIDs[k]]);
                    }
                    HandleSonicVertexPositions(SonicAni.frameIDs[j]);

                    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                    for (int l = j; SonicAni.frameIDs[l] < 254; ++l) {
                        MatrixMultiply(&MatrixSonicModel, &MatrixSonicAni_4C8990[SonicAni.frameIDs[l]]);
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
                    memcpy(&MatrixSonicModel, &MatrixSonicAni_4C9290[SonicAni.frameIDs[j]], sizeof(MatrixSonicModel));
                    HandleSonicVertexPositions(SonicAni.frameIDs[j]);

                    memcpy(&MatrixSonicModel, &MatrixSonicAni_4C8990[SonicAni.frameIDs[j]], sizeof(MatrixSonicModel));
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

            memcpy(&MatrixSonicAni_4C8990[j], &MatrixIdentity, sizeof(D3DMATRIX));
            WorldMatrixRotateZ(node->rotX[i]);

            MatrixMultiply(&MatrixSonicAni_4C8990[j], &MatrixWorld);
            WorldMatrixRotateY(node->rotY[i]);

            MatrixMultiply(&MatrixSonicAni_4C8990[j], &MatrixWorld);
            WorldMatrixRotateX(node->rotZ[i]);

            MatrixMultiply(&MatrixSonicAni_4C8990[j], &MatrixWorld);
            memcpy(&MatrixSonicAni_4C9290[j], &MatrixIdentity, sizeof(D3DMATRIX));

            WorldMatrixTranslateXYZ(-node->position.x, -node->position.y, -node->position.z);
            MatrixMultiply(&MatrixSonicAni_4C9290[j], &MatrixWorld);
            MatrixMultiply(&MatrixSonicAni_4C9290[j], &MatrixSonicAni_4C8990[j]);
            WorldMatrixTranslateXYZ(node->position.x, node->position.y, node->position.z);
            MatrixMultiply(&MatrixSonicAni_4C9290[j], &MatrixWorld);
        }

        for (int k = 0; k < SonicAni.frameIDCount; ++k) {
            if (v6 != false) {
                switch (SonicAni.frameIDs[k]) {
                    case 254: v6 = true; break;
                    case 255: v6 = false; break;

                    default:
                        memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                        for (int m = k; SonicAni.frameIDs[m] < 254; ++m) {
                            MatrixMultiply(&MatrixSonicModel, &MatrixSonicAni_4C9290[SonicAni.frameIDs[m]]);
                        }
                        memcpy(&matrix_47A790[k][i], &MatrixSonicModel, sizeof(matrix_47A790[k][i]));

                        memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
                        for (int n = k; SonicAni.frameIDs[n] < 254; ++n) {
                            MatrixMultiply(&MatrixSonicModel, &MatrixSonicAni_4C8990[SonicAni.frameIDs[n]]);
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
                        memcpy(&MatrixSonicModel, &MatrixSonicAni_4C9290[SonicAni.frameIDs[k]], sizeof(MatrixSonicModel));
                        HandleSonicVertexPositions(SonicAni.frameIDs[k]);

                        memcpy(&MatrixSonicModel, &MatrixSonicAni_4C8990[SonicAni.frameIDs[k]], sizeof(MatrixSonicModel));
                        HandleSonicVertexNormals(SonicAni.frameIDs[k]);
                        break;
                }
            }
        }
    }
}