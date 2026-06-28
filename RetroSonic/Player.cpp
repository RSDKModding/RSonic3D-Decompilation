#include "RetroEngine.hpp"

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
                data_4C9F68          = data_4C9F68 - Cos(data_4C9F74);
                Player[0].position.z = Player[0].position.z - Sin(data_4C9F74);
                data_4C9F70          = data_4C9F70 - Sin(data_4C9F74);
            }
            if (MGameInput.right == 1) {
                Player[0].position.x = Cos(data_4C9F74) + Player[0].position.x;
                data_4C9F68          = Cos(data_4C9F74) + data_4C9F68;
                Player[0].position.z = Sin(data_4C9F74) + Player[0].position.z;
                data_4C9F70          = Sin(data_4C9F74) + data_4C9F70;
            }
            if (MGameInput.Z) {
                if (MGameInput.up == 1) {
                    Player[0].position.y = Player[0].position.y + 1.0;
                    data_4C9F6C          = data_4C9F6C + 1.2;
                }
                if (MGameInput.down == 1) {
                    Player[0].position.y = Player[0].position.y - 1.0;
                    data_4C9F6C          = data_4C9F6C - 1.2;
                }
            }
            else {
                if (MGameInput.up == 1) {
                    v2                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.x = Player[0].position.x - Cos(v2);
                    v3                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F68          = data_4C9F68 - Cos(v3);
                    v4                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.z = Player[0].position.z - Sin(v4);
                    v5                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F70          = data_4C9F70 - Sin(v5);
                }
                if (MGameInput.down == 1) {
                    v6                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.x = Cos(v6) + Player[0].position.x;
                    v7                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F68          = Cos(v7) + data_4C9F68;
                    v8                   = data_4C9F74 - 3.1415927 * 0.5;
                    Player[0].position.z = Sin(v8) + Player[0].position.z;
                    v9                   = data_4C9F74 - 3.1415927 * 0.5;
                    data_4C9F70          = Sin(v9) + data_4C9F70;
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
        data_4C9F68 = data_4C9F68 - Player[PNumber].position.x;
        data_4C9F70 = data_4C9F70 - Player[PNumber].position.z;
        v9          = Cos(0.02) * data_4C9F68;
        v12         = Sin(0.02) * data_4C9F70 + v9;
        v8          = -Sin(0.02) * data_4C9F68;
        v10         = Cos(0.02) * data_4C9F70 + v8;
        data_4C9F68 = v12 + Player[PNumber].position.x;
        data_4C9F70 = v10 + Player[PNumber].position.z;
    }

    if (MGameInput.X == true) {
        data_4C9F68 = data_4C9F68 - Player[PNumber].position.x;
        data_4C9F70 = data_4C9F70 - Player[PNumber].position.z;
        v7          = Cos(-0.02) * data_4C9F68;
        v13         = Sin(-0.02) * data_4C9F70 + v7;
        v6          = -Sin(-0.02) * data_4C9F68;
        v11         = Cos(-0.02) * data_4C9F70 + v6;
        data_4C9F68 = v13 + Player[PNumber].position.x;
        data_4C9F70 = v11 + Player[PNumber].position.z;
    }

    if (data_4C9F68 == Player[PNumber].position.x) {
        if (data_4C9F70 >= (double)Player[PNumber].position.z)
            data_4C9F74 = 3.1415927;
        else
            data_4C9F74 = 0.0;
    }
    else {
        v4 = (data_4C9F70 - Player[PNumber].position.z) / (data_4C9F68 - Player[PNumber].position.x);
        if (data_4C9F68 <= (double)Player[PNumber].position.x)
            data_4C9F74 = ATan(v4) - 3.1415927 * 0.5;
        else
            data_4C9F74 = ATan(v4) + 3.1415927 * 0.5;
    }

    v5  = Player[PNumber].position.x - data_4C9F68;
    v14 = v5 / Sin(data_4C9F74);
    if (v14 < 0.0)
        v14 = v14 * -1.0;

    if (v14 < 32.0f) {
        data_4C9F68 = Player[PNumber].position.x - Sin(data_4C9F74) * -32.0;
        data_4C9F70 = Cos(data_4C9F74) * -32.0 + Player[PNumber].position.z;
    }

    if (v14 > 60.0f) {
        data_4C9F68 = Player[PNumber].position.x - Sin(data_4C9F74) * -60.0;
        data_4C9F70 = Cos(data_4C9F74) * -60.0 + Player[PNumber].position.z;
    }
}

void ProcessPlayerMovement()
{
    float a6; // [esp+0h] [ebp-14h]
    float v1; // [esp+4h] [ebp-10h]
    float v2; // [esp+4h] [ebp-10h]
    char v3;  // [esp+8h] [ebp-Ch]

    if (Player[PNumber].f_0x38 == 1 && Player[PNumber].f_0x48 == 1) {
        if (Player[PNumber].f_0x28 < Player[PNumber].f_0x2C) {
            Player[PNumber].f_0x28 += 6;
            if (Player[PNumber].f_0x24 > 0.0)
                Player[PNumber].f_0x24 -= 0.02;
            if (Player[PNumber].f_0x28 > Player[PNumber].f_0x2C)
                Player[PNumber].f_0x28 = Player[PNumber].f_0x2C;
        }

        if (Player[PNumber].f_0x28 > Player[PNumber].f_0x2C) {
            Player[PNumber].f_0x28 -= 6;
            if (Player[PNumber].f_0x24 > 0.0)
                Player[PNumber].f_0x24 -= 0.02;
            if (Player[PNumber].f_0x28 < Player[PNumber].f_0x2C)
                Player[PNumber].f_0x28 = Player[PNumber].f_0x2C;
        }

        if (Player[PNumber].f_0x24 < 2.4000001)
            Player[PNumber].f_0x24 += 0.012;
    }
    else if (Player[PNumber].f_0x38 == 0 && Player[PNumber].f_0x48 == 1) {
        if (Player[PNumber].f_0x28 < Player[PNumber].f_0x2C) {
            Player[PNumber].f_0x28 += 8;
            if (Player[PNumber].f_0x24 > 0.0)
                Player[PNumber].f_0x24 -= 0.0099999998;
            if (Player[PNumber].f_0x28 > Player[PNumber].f_0x2C)
                Player[PNumber].f_0x28 = Player[PNumber].f_0x2C;
        }

        if (Player[PNumber].f_0x28 > Player[PNumber].f_0x2C) {
            Player[PNumber].f_0x28 -= 8;
            if (Player[PNumber].f_0x24 > 0.0)
                Player[PNumber].f_0x24 -= 0.0099999998;
            if (Player[PNumber].f_0x28 < Player[PNumber].f_0x2C)
                Player[PNumber].f_0x28 = Player[PNumber].f_0x2C;
        }

        if (Player[PNumber].f_0x24 < 2.4000001)
            Player[PNumber].f_0x24 = Player[PNumber].f_0x24 + 0.012;
    }
    else {
        if (Player[PNumber].f_0x24 > 0.0)
            Player[PNumber].f_0x24 -= 0.012;

        if (Player[PNumber].f_0x24 > -0.050000001 && Player[PNumber].f_0x24 < 0.0)
            Player[PNumber].f_0x24 = 0.0;
    }

    if (Player[PNumber].f_0x38 != 2) {
        if (Player[PNumber].f_0x44 == 1) {
            Player[PNumber].f_0x38 = 1;

            Player[PNumber].velocity.y -= 0.050000001;
            if (Player[PNumber].velocity.y > 4.0) {
                Player[PNumber].velocity.y = 4.0;
                Unknown_408222();
            }

            v1 = Player[PNumber].f_0x24 * 0.40000001 + 0.40000001;
            SonicModel_405CE2(4, v1);
        }
        else {
            if (Player[0].f_0x24 < 0.0099999998) {
                SonicModel_405CE2(0, 0.0);
            }
            else {
                v2 = Player[PNumber].f_0x24 * 0.40000001;
                SonicModel_405CE2(2, v2);
            }

            Player[PNumber].f_0x38 = 0;
            if (Player[PNumber].f_0x24 == 0.0)
                Player[PNumber].gap_54[0] = 0;

            if (Player[PNumber].f_0x4C == 1) {
                Player[PNumber].f_0x44     = 1;
                Player[PNumber].velocity.y = 2.0;
                Player[PNumber].f_0x38     = 1;
                Unknown_408222();
            }
        }
    }

    Player[PNumber].velocity.x = -Sin(Player[PNumber].f_0x30) * Player[PNumber].f_0x24;
    Player[PNumber].velocity.z = Cos(Player[PNumber].f_0x30) * Player[PNumber].f_0x24;
    Player[PNumber].f_0x18.x   = 0.0;
    Player[PNumber].f_0x18.y   = -4.0;
    Player[PNumber].f_0x18.z   = 0.0;

    if (Player[PNumber].f_0x44 == 1) {
        if (Player[PNumber].velocity.y < 0.0) {
            a6 = Player[PNumber].velocity.y * -1.0;
            if (ObjectFloorCollision(&Player[PNumber].position.x, &Player[PNumber].position.y, &Player[PNumber].position.z,
                                     Player[PNumber].velocity.x, a6, Player[PNumber].velocity.z)
                == 1) {
                Player[PNumber].velocity.y = 0.0;
                Player[PNumber].f_0x44     = 0;
                Player[PNumber].f_0x38     = 0;
            }
        }
    }
    else {
        Player[PNumber].velocity.y = 0.0;
        Matrix_40812C(&Player[PNumber].velocity.x, &Player[PNumber].velocity.y, &Player[PNumber].velocity.z);
        Player[PNumber].f_0x18.x = 0.0;
        Player[PNumber].f_0x18.y = -3.5;
        Player[PNumber].f_0x18.z = 0.0;
        Matrix_40812C(&Player[PNumber].f_0x18.x, &Player[PNumber].f_0x18.y, &Player[PNumber].f_0x18.z);
        Player[PNumber].position.x = Player[PNumber].position.x - (Player[PNumber].f_0x18.x - Player[PNumber].f_0x18.x * 0.25);
        Player[PNumber].position.y = Player[PNumber].position.y - (Player[PNumber].f_0x18.y - Player[PNumber].f_0x18.y * 0.25);
        Player[PNumber].position.z = Player[PNumber].position.z - (Player[PNumber].f_0x18.z - Player[PNumber].f_0x18.z * 0.25);

        v3 = ObjectFloorCollision(&Player[PNumber].position.x, &Player[PNumber].position.y, &Player[PNumber].position.z, Player[PNumber].f_0x18.x,
                                  Player[PNumber].f_0x18.y, Player[PNumber].f_0x18.z);
        if (v3) {
            if (v3 > 0 && v3 <= 2) {
                Player[PNumber].f_0x44 = 0;
                Player[PNumber].f_0x38 = 0;
            }
        }
        else {
            Player[PNumber].position.x = Player[PNumber].f_0x18.x - Player[PNumber].f_0x18.x * 0.25 + Player[PNumber].position.x;
            Player[PNumber].position.y = Player[PNumber].f_0x18.y - Player[PNumber].f_0x18.y * 0.25 + Player[PNumber].position.y;
            Player[PNumber].position.z = Player[PNumber].f_0x18.z - Player[PNumber].f_0x18.z * 0.25 + Player[PNumber].position.z;
            Player[PNumber].f_0x44     = 1;
        }
    }

    Player[PNumber].position.x += Player[PNumber].velocity.x;
    Player[PNumber].position.y += Player[PNumber].velocity.y;
    Player[PNumber].position.z += Player[PNumber].velocity.z;
    ProcessPlayerCamera();

    if (Player[PNumber].f_0x48 == 1)
        Player[PNumber].f_0x30 = (double)Player[PNumber].f_0x28 / 128.0 * 3.1415927 + data_4C9F74;

    if (Player[PNumber].f_0x44 == 1) {
        if (data_4C9F7C < 30)
            data_4C9F7C++;

        if (data_4C9F78 < (double)Player[PNumber].velocity.y) {
            data_4C9F78 += 0.1;
            if (data_4C9F78 > (double)Player[PNumber].velocity.y)
                data_4C9F78 = Player[PNumber].velocity.y;
        }

        if (data_4C9F78 > (double)Player[PNumber].velocity.y) {
            data_4C9F78 -= 0.1;
            if (data_4C9F78 < (double)Player[PNumber].velocity.y)
                data_4C9F78 = Player[PNumber].velocity.y;
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

    data_4C9F6C = Player[PNumber].position.y + 15.0 - (double)data_4C9F7C * 0.25 * data_4C9F78;
}

void LoadPlayerGfx(const char *textureName, sbyte playerID)
{
    char name[64];
    lstrcpy(name, "Data/Characters/");
    lstrcat(name, textureName);

    if (surfaceCharacters[playerID] != NULL)
        surfaceCharacters[playerID]->Release();

    surfaceCharacters[playerID] = CreateTexture(D3DDevice, name);

    if (playerID == 0)
        DDLoadBitmap(name, 0);
}

void SonicModel_405A18(int frameID)
{
    AnimationFrame *frame = &SonicAni.frames[frameID];

    for (int i = 0; i < frame->vertexCount; ++i) {
        D3DVERTEX *vert = &SonicMdl.vertices[frame->vertexIDs[i]];
        D3DVERTEX *base = &SonicBaseMdl.vertices[frame->vertexIDs[i]];

        vert->x = matSonicMdl.m[0][0] * base->x + matSonicMdl.m[1][0] * base->y + matSonicMdl.m[2][0] * base->z + matSonicMdl.m[3][0];
        vert->y = matSonicMdl.m[0][1] * base->x + matSonicMdl.m[1][1] * base->y + matSonicMdl.m[2][1] * base->z + matSonicMdl.m[3][1];
        vert->z = matSonicMdl.m[0][2] * base->x + matSonicMdl.m[1][2] * base->y + matSonicMdl.m[2][2] * base->z + matSonicMdl.m[3][2];
    }
}

void SonicModel_405B7B(int frameID)
{
    AnimationFrame *frame = &SonicAni.frames[frameID];

    for (int i = 0; i < frame->vertexCount; ++i) {
        D3DVERTEX *vert = &SonicMdl.vertices[frame->vertexIDs[i]];
        D3DVERTEX *base = &SonicBaseMdl.vertices[frame->vertexIDs[i]];

        vert->nx = matSonicMdl.m[0][0] * base->nx + matSonicMdl.m[1][0] * base->ny + matSonicMdl.m[2][0] * base->nz + matSonicMdl.m[3][0];
        vert->ny = matSonicMdl.m[0][1] * base->nx + matSonicMdl.m[1][1] * base->ny + matSonicMdl.m[2][1] * base->nz + matSonicMdl.m[3][1];
        vert->nz = matSonicMdl.m[0][2] * base->nx + matSonicMdl.m[1][2] * base->ny + matSonicMdl.m[2][2] * base->nz + matSonicMdl.m[3][2];
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
        SonicAni.array_AB90[2].field_201 = (__int64)(a2 * 128.0);
    }
    else if (a1 == 4) {
        data_4C9D4C = data_4C9D4C - a2 * 0.30000001f;
        if (data_4C9D4C < 0.0f)
            data_4C9D4C = 3.1415927f + 3.1415927f;
        SonicAni.array_AB90[4].field_201 = (__int64)(a2 * 128.0);
    }
}

void ProcessPlayerAnimationLMC()
{
    int v0;       // edx
    float a1;     // [esp+0h] [ebp-30h]
    float a1_4;   // [esp+4h] [ebp-2Ch]
    float a2;     // [esp+8h] [ebp-28h]
    int i;        // [esp+1Ch] [ebp-14h]
    int j;        // [esp+1Ch] [ebp-14h]
    float v6;     // [esp+20h] [ebp-10h]
    char v7;      // [esp+24h] [ebp-Ch]
    float value;  // [esp+28h] [ebp-8h]
    float valuea; // [esp+28h] [ebp-8h]
    float valueb; // [esp+28h] [ebp-8h]
    int k;        // [esp+2Ch] [ebp-4h]
    int l;        // [esp+2Ch] [ebp-4h]

    v7 = 0;
    if (SonicAni.field_BFAA == SonicAni.field_BFAB)
        v0 = SonicAni.field_BFAA;
    else
        v0 = SonicAni.field_BFAB;
    SonicAni.field_BFB0 += SonicAni.array_AB90[v0].field_201;
    if (SonicAni.field_BFB0 > 0xEFu) {
        SonicAni.field_BFB0 -= 240;
        if (++SonicAni.field_BFAC >= (int)SonicAni.array_AB90[SonicAni.field_BFAA].count)
            SonicAni.field_BFAC = SonicAni.array_AB90[SonicAni.field_BFAA].field_200;
        if (SonicAni.field_BFAA != SonicAni.field_BFAB) {
            SonicAni.field_BFAA = SonicAni.field_BFAB;
            SonicAni.field_BFAC = 0;
        }
        SonicAni.field_BFAE = SonicAni.field_BFAC + 1;
        if (SonicAni.field_BFAE >= (int)SonicAni.array_AB90[SonicAni.field_BFAA].count)
            SonicAni.field_BFAE = SonicAni.array_AB90[SonicAni.field_BFAA].field_200;
    }
    v6 = (double)SonicAni.field_BFB0 / 240.0;
    for (i = 0; i < 36; ++i) {
        memcpy(&matrix_4C8990[i], &matWorld, sizeof(D3DMATRIX));
        value = (1.0 - v6) * SonicAni.frames[i].rotX[SonicAni.array_AB90[SonicAni.field_BFAA].array_2[SonicAni.field_BFAC]]
                + v6 * SonicAni.frames[i].rotX[SonicAni.array_AB90[SonicAni.field_BFAB].array_2[SonicAni.field_BFAE]];
        MatrixRotateX_4C9DB0(value);
        MatrixMultiply(&matrix_4C8990[i], &matrixBackgroundTransform);
        valuea = (1.0 - v6) * SonicAni.frames[i].rotY[SonicAni.array_AB90[SonicAni.field_BFAA].array_2[SonicAni.field_BFAC]]
                 + v6 * SonicAni.frames[i].rotY[SonicAni.array_AB90[SonicAni.field_BFAB].array_2[SonicAni.field_BFAE]];
        MatrixRotateY_4C9DB0(valuea);
        MatrixMultiply(&matrix_4C8990[i], &matrixBackgroundTransform);
        valueb = (1.0 - v6) * SonicAni.frames[i].rotZ[SonicAni.array_AB90[SonicAni.field_BFAA].array_2[SonicAni.field_BFAC]]
                 + v6 * SonicAni.frames[i].rotZ[SonicAni.array_AB90[SonicAni.field_BFAB].array_2[SonicAni.field_BFAE]];
        MatrixRotateZ_4C9DB0(valueb);
        MatrixMultiply(&matrix_4C8990[i], &matrixBackgroundTransform);
        memcpy(&matrix_4C9290[i], &matWorld, sizeof(D3DMATRIX));
        a2   = -SonicAni.frames[i].position.z;
        a1_4 = -SonicAni.frames[i].position.y;
        a1   = -SonicAni.frames[i].position.x;
        Matrix_408B0B(a1, a1_4, a2);
        MatrixMultiply(&matrix_4C9290[i], &matrixBackgroundTransform);
        MatrixMultiply(&matrix_4C9290[i], &matrix_4C8990[i]);
        Matrix_408B0B(SonicAni.frames[i].position.x, SonicAni.frames[i].position.y, SonicAni.frames[i].position.z);
        MatrixMultiply(&matrix_4C9290[i], &matrixBackgroundTransform);
    }
    for (j = 0; j < SonicAni.field_BFA8; ++j) {
        if (v7) {
            if (SonicAni.frameIDs[j] == 255) {
                v7 = 0;
            }
            else if (SonicAni.frameIDs[j] == 254) {
                v7 = 1;
            }
            else {
                memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                for (k = j; SonicAni.frameIDs[k] < 0xFEu; ++k) MatrixMultiply(&matSonicMdl, &matrix_4C9290[SonicAni.frameIDs[k]]);
                SonicModel_405A18(SonicAni.frameIDs[j]);
                memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                for (l = j; SonicAni.frameIDs[l] < 254u; ++l) MatrixMultiply(&matSonicMdl, &matrix_4C8990[SonicAni.frameIDs[l]]);
                SonicModel_405B7B(SonicAni.frameIDs[j]);
            }
        }
        else if (SonicAni.frameIDs[j] == 254) {
            v7 = 1;
        }
        else if (SonicAni.frameIDs[j] == 255) {
            v7 = 0;
        }
        else {
            memcpy(&matSonicMdl, &matrix_4C9290[SonicAni.frameIDs[j]], sizeof(matSonicMdl));
            SonicModel_405A18(SonicAni.frameIDs[j]);
            memcpy(&matSonicMdl, &matrix_4C8990[SonicAni.frameIDs[j]], sizeof(matSonicMdl));
            SonicModel_405B7B(SonicAni.frameIDs[j]);
        }
    }
}

void MightBeSonicAnim_406432()
{
    float v0; // [esp+0h] [ebp-28h]
    float a2; // [esp+4h] [ebp-24h]
    float v2; // [esp+8h] [ebp-20h]
    int j;    // [esp+18h] [ebp-10h]
    int k;    // [esp+18h] [ebp-10h]
    int i;    // [esp+1Ch] [ebp-Ch]
    char v6;  // [esp+20h] [ebp-8h]
    int m;    // [esp+24h] [ebp-4h]
    int n;    // [esp+24h] [ebp-4h]

    v6 = 0;
    for (i = 0; i < 36; ++i) {
        for (j = 0; j < 36; ++j) {
            memcpy(&matrix_4C8990[j], &matWorld, sizeof(D3DMATRIX));
            MatrixRotateX_4C9DB0(SonicAni.frames[j].rotX[i]);

            MatrixMultiply(&matrix_4C8990[j], &matrixBackgroundTransform);
            MatrixRotateY_4C9DB0(SonicAni.frames[j].rotY[i]);

            MatrixMultiply(&matrix_4C8990[j], &matrixBackgroundTransform);
            MatrixRotateZ_4C9DB0(SonicAni.frames[j].rotZ[i]);

            MatrixMultiply(&matrix_4C8990[j], &matrixBackgroundTransform);
            memcpy(&matrix_4C9290[j], &matWorld, sizeof(D3DMATRIX));

            Matrix_408B0B(-SonicAni.frames[j].position.x, -SonicAni.frames[j].position.y, -SonicAni.frames[j].position.z);
            MatrixMultiply(&matrix_4C9290[j], &matrixBackgroundTransform);
            MatrixMultiply(&matrix_4C9290[j], &matrix_4C8990[j]);
            Matrix_408B0B(SonicAni.frames[j].position.x, SonicAni.frames[j].position.y, SonicAni.frames[j].position.z);
            MatrixMultiply(&matrix_4C9290[j], &matrixBackgroundTransform);
        }

        for (k = 0; k < SonicAni.field_BFA8; ++k) {
            if (v6) {
                if (SonicAni.frameIDs[k] == 255) {
                    v6 = 0;
                }
                else if (SonicAni.frameIDs[k] == 254) {
                    v6 = 1;
                }
                else {
                    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                    for (m = k; SonicAni.frameIDs[m] < 254u; ++m) MatrixMultiply(&matSonicMdl, &matrix_4C9290[SonicAni.frameIDs[m]]);
                    memcpy(&matrix_47A790[k][i], &matSonicMdl, sizeof(matrix_47A790[k][i]));
                    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                    for (n = k; SonicAni.frameIDs[n] < 254u; ++n) MatrixMultiply(&matSonicMdl, &matrix_4C8990[SonicAni.frameIDs[n]]);
                    memcpy(&array_42C590[k][i], &matSonicMdl, sizeof(array_42C590[k][i]));
                }
            }
            else if (SonicAni.frameIDs[k] == 254) {
                v6 = 1;
            }
            else if (SonicAni.frameIDs[k] == 255) {
                v6 = 0;
            }
            else {
                memcpy(&matSonicMdl, &matrix_4C9290[SonicAni.frameIDs[k]], sizeof(matSonicMdl));
                SonicModel_405A18(SonicAni.frameIDs[k]);
                memcpy(&matSonicMdl, &matrix_4C8990[SonicAni.frameIDs[k]], sizeof(matSonicMdl));
                SonicModel_405B7B(SonicAni.frameIDs[k]);
            }
        }
    }
}