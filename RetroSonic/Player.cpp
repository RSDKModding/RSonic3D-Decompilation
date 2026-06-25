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
                InitObjectUnknown(4, 0, Player[0].position.x, a4, Player[0].position.z);
            }

            if (MGameInput.X == 1) {
                a4a = Player[0].position.y + 4.0;
                InitObjectUnknown(1, 0, Player[0].position.x, a4a, Player[0].position.z);
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
            if (ObjectFloorCollision(&Player[PNumber].position.x, &Player[PNumber].position.y, &Player[PNumber].position.z, Player[PNumber].velocity.x, a6,
                               Player[PNumber].velocity.z)
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