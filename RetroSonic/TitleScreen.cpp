#include "RetroEngine.hpp"

InputData TitleInput;
TextMenu TitleMenu;
float data_4DA320;
float backpos[10];
float TYspeed;
float TXspeed;
float TailsY;
float TailsX;
int TAnimation;
int SAnimation;
float SYspeed;
float SXspeed;
float SonY;
float SonX;
int TitleScreenMode;
int TtlTime;
char TxtStyle;
float RysS;
float SwS;
int AllStages;
uint8_t CodePos;
int SMenuY1;
int SMenuY2;
int MusicNo;
int SStageNo;
float data_4DA3A8;

float data_41F59C = 70.0f;
int DebugEnabled  = TRUE;
int DebugMode     = TRUE;

uint8_t DebugCode[12] = { 2, 2, 3, 3, 0, 1, 0, 1, 4, 0, 0, 0 };

void ProcessTitleScreen()
{
    switch (TitleScreenMode) {
        case 0:
            ++TtlTime;
            CodeCheck();
            Render_ClearScreen(0x000000);

            if (TtlTime == 240) {
                TtlTime = 0;
                ++TitleScreenMode;
                backpos[0] = -0.5f;
                backpos[1] = -1.0f;
                backpos[2] = -0.1f;
                backpos[3] = -0.40000001f;
                backpos[4] = -0.69999999f;
                backpos[5] = -0.2f;
                backpos[6] = -0.40000001f;
                backpos[7] = -0.30000001f;
                backpos[8] = -0.80000001f;
                backpos[9] = -1.0f;
                SonX       = 130.0f;
                SonY       = 142.0f;
                SYspeed    = -2.0f;
                SXspeed    = 0.0f;
                TXspeed    = 0.0f;
                SAnimation = 0;
                TAnimation = 0;
                TailsX     = 100.0f;
                TailsY     = 146.0f;
                TYspeed    = -2.0f;
                SMenuY1    = 0;
                SMenuY2    = 0;
                RysS       = 80.0f;
                SwS        = 8.0f;
                TxtStyle   = 0;

                if (DebugEnabled == TRUE) {
                    TextMeDo("NEW GAME", &TitleMenu, 4, 0, 8);
                    SetGameText("CONTINUE GAME", &TitleMenu, 1, 13);
                    SetGameText("DEBUG OPTIONS", &TitleMenu, 2, 13);
                    SetGameText("EXIT GAME", &TitleMenu, 3, 9);
                }
                else {
                    TextMeDo("STANDING", &TitleMenu, 3, 0, 8);
                    SetGameText("WAITING", &TitleMenu, 1, 7);
                    SetGameText("WALKING", &TitleMenu, 2, 7);
                }

                TitleMenu.field_C  = 2;
                TitleMenu.field_D  = 1;
                TitleMenu.field_10 = 0;
                SetFade(1.0f, 1.0f, 1.0f, 0.89999998f);
            }
            else {
                DrawTitleScr(0);

                if (TtlTime < 61)
                    SetFade(0.0f, 0.0f, 0.0f, 1.0f - (double)TtlTime * 0.0166f);

                if (TtlTime > 179)
                    SetFade(1.0f, 1.0f, 1.0f, (double)(TtlTime - 180) * 0.0166f);
            }
            break;

        case 1:
            ++TtlTime;
            TitleScrMovement();
            DrawTitleScr(1);
            Zone_TitleScreen_4127E6();
            DrawTitleScr(0);

            if (TtlTime < 61)
                SetFade(1.0f, 1.0f, 1.0f, 1.0f - (double)TtlTime * 0.0166f);

            if (TtlTime == 180) {
                TtlTime = 0;
                ++TitleScreenMode;
            }
            break;

        case 2:
            TitleScrMovement();
            DrawTitleScr(1);
            Zone_TitleScreen_4127E6();
            DrawTitleScr(0);

            CheckInput(&TitleInput);

            if (TitleInput.left == TRUE)
                data_4DA3A8 -= 0.02f;

            if (TitleInput.right == TRUE)
                data_4DA3A8 += 0.02f;

            if (data_4DA3A8 < 0.0f)
                data_4DA3A8 = 3.1415927f + 3.1415927f;

            if (3.1415927f + 3.1415927f < data_4DA3A8)
                data_4DA3A8 = 0.0f;

            SonicModel_405CE2(TitleMenu.field_10 + 2, data_4DA3A8);

            CheckKeyPress(&TitleInput, INPUT_LEFT, INPUT_LCONTROL);

            if (TitleInput.down == TRUE && TitleMenu.field_10 < TitleMenu.rowCount - 1)
                TitleMenu.field_10++;

            if (TitleInput.up == TRUE && TitleMenu.field_10 > 0)
                TitleMenu.field_10--;

            if (TitleInput.start == TRUE) {
                switch (TitleMenu.field_10) {
                    case 0:
                        TitleMenu       = LoadCharacterMenu();
                        TitleScreenMode = 3;
                        break;

                    case 1: TitleScreenMode = 5; break;

                    case 2:
                        TitleMenu       = LoadCharacterMenu();
                        TitleScreenMode = 3;
                        break;

                    case 3:
                        if (DebugEnabled == TRUE) {
                            TextMeDo("DEBUG MENU", &TitleMenu, 12, 0, 10);
                            SetGameText(" ", &TitleMenu, 1, 1);
                            SetGameText(" ", &TitleMenu, 2, 1);

                            if (AllStages)
                                SetGameText("ALL ZONES eONe ", &TitleMenu, 3, 15);
                            else
                                SetGameText("ALL ZONES eOFFe", &TitleMenu, 3, 15);

                            SetGameText(" ", &TitleMenu, 4, 1);

                            if (DebugMode)
                                SetGameText("DEBUG MODE eONe ", &TitleMenu, 5, 16);
                            else
                                SetGameText("DEBUG MODE eOFFe", &TitleMenu, 5, 16);

                            SetGameText(" ", &TitleMenu, 6, 1);
                            SetGameText("PLAY SPECIAL STAGE e00e", &TitleMenu, 7, 23);

                            SetGameText(" ", &TitleMenu, 8, 1);
                            SetGameText("SOUND TEST e00e", &TitleMenu, 9, 15);

                            SetGameText(" ", &TitleMenu, 10, 1);
                            SetGameText("eEXITe", &TitleMenu, 11, 6);

                            TitleMenu.field_C  = 2;
                            TitleMenu.field_D  = 2;
                            TitleMenu.field_10 = 0;
                            TitleMenu.field_14 = 3;
                            TitleScreenMode    = 6;
                        }
                        break;

                    default: break;
                }
            }

            Zone_TitleScreen_4128A3();
            break;

        case 3:
            Render_ClearScreen(0x000000);
            DrawTitleScr(1);
            DrawTitleScr(0);

            CheckKeyPress(&TitleInput, INPUT_UP, INPUT_LCONTROL);

            if (TitleInput.down == TRUE && TitleMenu.field_10 < TitleMenu.rowCount - 1)
                TitleMenu.field_10++;

            if (TitleInput.up == TRUE && TitleMenu.field_10 > 0)
                TitleMenu.field_10--;

            if (TitleInput.start == TRUE) {
                TitleMenu       = LoadZoneMenu();
                TitleScreenMode = 4;
            }

            Zone_TitleScreen_4128CF();
            break;

        case 4:
            Render_ClearScreen(0x000000);
            DrawTitleScr(1);
            DrawTitleScr(0);

            CheckKeyPress(&TitleInput, INPUT_UP, INPUT_LCONTROL);

            if (TitleInput.down == TRUE && TitleMenu.field_10 < TitleMenu.rowCount - 1)
                ++TitleMenu.field_10;

            if (TitleInput.up == TRUE && TitleMenu.field_10 > 0)
                --TitleMenu.field_10;

            if (TitleInput.start == TRUE) {
                TitleScreenMode = 4;
                SetShouldSkipTitle(TRUE);
            }

            DrawMenuBackground();
            break;

        case 6:
            Render_ClearScreen(0x000000);
            DrawTitleScr(1);
            DrawTitleScr(0);

            CheckKeyPress(&TitleInput, INPUT_LEFT, INPUT_LSHIFT);

            if (TitleInput.down == TRUE && TitleMenu.field_14 < TitleMenu.rowCount - 1)
                TitleMenu.field_14 += 2;

            if (TitleInput.up == TRUE && TitleMenu.field_14 > 3)
                TitleMenu.field_14 -= 2;

            if (TitleInput.left == TRUE) {
                switch (TitleMenu.field_14) {
                    case 3:
                        if (AllStages) {
                            AllStages = FALSE;
                            memcpy(TitleMenu.labels[3].text + 12, "\x06\x06%", 3);
                        }
                        else {
                            AllStages = TRUE;
                            strcpy(TitleMenu.labels[3].text + 12, "\x0E%");
                        }
                        break;

                    case 5:
                        if (DebugMode) {
                            DebugMode = FALSE;
                            memcpy(TitleMenu.labels[5].text + 13, "\x06\x06%", 3);
                        }
                        else {
                            DebugMode                    = TRUE;
                            TitleMenu.labels[5].text[13] = 14;
                            TitleMenu.labels[5].text[14] = 37;
                            TitleMenu.labels[5].text[15] = 0;
                        }
                        break;

                    case 7:
                        if (SStageNo > 0) {
                            TitleMenu.labels[7].text[20] = --SStageNo / 10 + 27;
                            TitleMenu.labels[7].text[21] = SStageNo + 27 - 10 * (SStageNo / 10);
                        }
                        break;

                    case 9:
                        if (MusicNo > 0) {
                            TitleMenu.labels[9].text[12] = --MusicNo / 10 + 27;
                            TitleMenu.labels[9].text[13] = MusicNo + 27 - 10 * (MusicNo / 10);
                        }
                        break;
                    default: break;
                }
            }

            if (TitleInput.right == TRUE) {
                switch (TitleMenu.field_14) {
                    case 3:
                        if (AllStages) {
                            AllStages                    = 0;
                            TitleMenu.labels[3].text[12] = 6;
                            TitleMenu.labels[3].text[13] = 6;
                            TitleMenu.labels[3].text[14] = 37;
                        }
                        else {
                            AllStages                    = 1;
                            TitleMenu.labels[3].text[12] = 14;
                            TitleMenu.labels[3].text[13] = 37;
                            TitleMenu.labels[3].text[14] = 0;
                        }
                        break;

                    case 5:
                        if (DebugMode) {
                            DebugMode                    = FALSE;
                            TitleMenu.labels[5].text[13] = 6;
                            TitleMenu.labels[5].text[14] = 6;
                            TitleMenu.labels[5].text[15] = 37;
                        }
                        else {
                            DebugMode                    = TRUE;
                            TitleMenu.labels[5].text[13] = 14;
                            TitleMenu.labels[5].text[14] = 37;
                            TitleMenu.labels[5].text[15] = 0;
                        }
                        break;

                    case 7:
                        if (SStageNo < 99) {
                            TitleMenu.labels[7].text[20] = ++SStageNo / 10 + 27;
                            TitleMenu.labels[7].text[21] = SStageNo + 27 - 10 * (SStageNo / 10);
                        }
                        break;

                    case 9:
                        if (MusicNo < 99) {
                            TitleMenu.labels[9].text[12] = ++MusicNo / 10 + 27;
                            TitleMenu.labels[9].text[13] = MusicNo + 27 - 10 * (MusicNo / 10);
                        }
                        break;

                    default: break;
                }
            }

            if (TitleInput.start == TRUE) {
                if (TitleMenu.field_14 == 11)
                    TitleScreenMode = 0;
            }

            Zone_TitleScreen_4129E4();
            break;

        default: break;
    }

    FlipScreen();
}

void CodeCheck()
{
    CheckKeyPress(&TitleInput, INPUT_LEFT, INPUT_START);

    if (TitleInput.up == TRUE) {
        if (DebugCode[CodePos] == 2)
            ++CodePos;
        else
            CodePos = 0;
    }

    if (TitleInput.down == TRUE) {
        if (DebugCode[CodePos] == 3)
            ++CodePos;
        else
            CodePos = 0;
    }

    if (TitleInput.left == TRUE) {
        if (DebugCode[CodePos])
            CodePos = 0;
        else
            ++CodePos;
    }

    if (TitleInput.right == TRUE) {
        if (DebugCode[CodePos] == 1)
            ++CodePos;
        else
            CodePos = 0;
    }

    if (DebugCode[CodePos] == 4) {
        if (DebugEnabled == TRUE)
            DebugEnabled = FALSE;
        else if (DebugEnabled == FALSE)
            DebugEnabled = TRUE;

        CodePos = 0;
    }
}

void SetMenuSelMode(TextMenu *menu, int32_t id) { menu->selMode[id] = 1; }

TextMenu LoadCharacterMenu()
{
    // TODO
    return {};
}

TextMenu LoadZoneMenu()
{
    // TODO
    return {};
}

// TODO: MOVE
void DrawGameMenu(TextMenu textMenu, int32_t a7, uint8_t a8)
{
    int i;  // [esp+10h] [ebp-8h]
    int j;  // [esp+10h] [ebp-8h]
    int k;  // [esp+10h] [ebp-8h]
    int v6; // [esp+14h] [ebp-4h]
    int v7; // [esp+14h] [ebp-4h]

    switch (textMenu.field_C) {
        case 0u:
            for (i = 0;; ++i) {
                if (i >= textMenu.rowCount)
                    return;
                if (textMenu.field_D == 1)
                    break;
                if (textMenu.field_D == 2) {
                    if (i == textMenu.field_14 || i == textMenu.field_10) {
                    LABEL_13:
                        DrawText_1(textMenu.labels[i].text, textMenu.labels[i].length, a7, 10 * i + a8, 8);
                        continue;
                    }
                    goto LABEL_16;
                }
                if (textMenu.field_D == 3) {
                    if (i == textMenu.field_10)
                        DrawText_1(textMenu.labels[i].text, textMenu.labels[i].length, a7, 10 * i + a8, 8);
                    if (i == textMenu.field_14 && i != textMenu.field_10)
                        DrawText_2(textMenu.labels[i].text, textMenu.labels[i].length, a7, 10 * i + a8);
                }
            LABEL_6:;
            }
            if (i == textMenu.field_10)
                goto LABEL_13;
        LABEL_16:
            DrawText_1(textMenu.labels[i].text, textMenu.labels[i].length, a7, 10 * i + a8, 0);
            goto LABEL_6;
        case 1u:
            for (j = 0;; ++j) {
                if (j >= textMenu.rowCount)
                    return;
                v6 = a7 - 10 * textMenu.labels[j].length;
                if (textMenu.field_D == 1)
                    break;
                if (textMenu.field_D == 2) {
                    if (j == textMenu.field_14 || j == textMenu.field_10) {
                    LABEL_31:
                        DrawText_1(textMenu.labels[j].text, textMenu.labels[j].length, v6, 10 * j + a8, 8);
                        continue;
                    }
                    goto LABEL_34;
                }
            LABEL_25:;
            }
            if (j == textMenu.field_10)
                goto LABEL_31;
        LABEL_34:
            DrawText_1(textMenu.labels[j].text, textMenu.labels[j].length, v6, 10 * j + a8, 0);
            goto LABEL_25;
        case 2u:
            for (k = 0;; ++k) {
                if (k >= textMenu.rowCount)
                    return;
                v7 = a7 - 10 * ((signed int)textMenu.labels[k].length / 2);
                if (textMenu.field_D == 1)
                    break;
                if (textMenu.field_D == 2) {
                    if (k == textMenu.field_14 || k == textMenu.field_10) {
                    LABEL_43:
                        DrawText_1(textMenu.labels[k].text, textMenu.labels[k].length, v7, 10 * k + a8, 8);
                        continue;
                    }
                    goto LABEL_46;
                }
            LABEL_37:;
            }
            if (k == textMenu.field_10)
                goto LABEL_43;
        LABEL_46:
            DrawText_1(textMenu.labels[k].text, textMenu.labels[k].length, v7, 10 * k + a8, 0);
            goto LABEL_37;
    }
}

// TODO: MOVE
void DrawScrollingMenu(TextMenu p_0, int32_t p_1, int32_t p_2, int32_t p_3, int32_t p_4, int32_t p_5)
{
    //
}

void TitleScrMovement()
{
    if (TtlTime > 30 && SonX < 160.0f) {
        SXspeed     = SXspeed + 0.0099999998f;
        SYspeed     = SYspeed + 0.02f;
        SonX        = SonX + SXspeed;
        SonY        = SonY + SYspeed;
        data_41F59C = SYspeed * 0.5f + data_41F59C;
    }

    if (SonX > 160.0f && TailsX > 78.0f) {
        TXspeed = TXspeed - 0.0099999998f;
        TYspeed = TYspeed + 0.02f;
        TailsX  = TailsX + TXspeed;
        TailsY  = TailsY + TYspeed;
    }

    if (TtlTime > 30 && SonX > 160.0f && SAnimation < 9)
        ++SAnimation;

    if (TtlTime > 30 && TailsX < 78.0f && TAnimation < 11)
        ++TAnimation;
}

void DrawTitleScr(char type)
{
    switch (type) {
        case 0:
            IDirect3DDevice7_SetTransform(dx7Device, D3DTRANSFORMSTATE_WORLD, &matWorld);
            IDirect3DDevice7_SetTransform(dx7Device, D3DTRANSFORMSTATE_VIEW, &matWorld);
            IDirect3DDevice7_SetTransform(dx7Device, D3DTRANSFORMSTATE_PROJECTION, &matWorld);

            IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_ZENABLE, FALSE);
            IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_LIGHTING, FALSE);
            IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_SPECULARENABLE, FALSE);
            break;

        case 1:
            IDirect3DDevice7_SetTransform(dx7Device, D3DTRANSFORMSTATE_WORLD, &matWorld);
            IDirect3DDevice7_SetTransform(dx7Device, D3DTRANSFORMSTATE_VIEW, &matView);
            IDirect3DDevice7_SetTransform(dx7Device, D3DTRANSFORMSTATE_PROJECTION, &matProject);

            IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_ZENABLE, TRUE);
            IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_LIGHTING, TRUE);
            IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_SPECULARENABLE, FALSE);
            break;

        case 2: IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_ZENABLE, FALSE); break;
        case 3: IDirect3DDevice7_SetRenderState(dx7Device, D3DRENDERSTATE_ZENABLE, TRUE); break;

        default: break;
    }
}

void Zone_TitleScreen_4127E6()
{
    Render_ClearScreen(0x000000);

    data_4DA320 += 0.0049999999f;
    if (3.1415927 + 3.1415927 < data_4DA320)
        data_4DA320 = 0.0f;

    DrawTitleScr(2);
    CopyMatrix_4C9B90_4C9C50();
    MatrixRotateY_4C9DB0(data_4DA320);
    MultiplyMatrix_4C9B90_4C9BD0();
    Matrix_408B0B(0.0f, 0.0f, 20.0f);
    MultiplyMatrix_4C9B90_4C9BD0();
    SonicMat_WorldTransform();
    DrawTitleModel(0);
    DrawTitleScr(3);
    CopyMatrix_4C9B90_4C9C50();
    Matrix_408B0B(0.0f, 0.0f, data_41F59C);
    MultiplyMatrix_4C9B90_4C9BD0();
    SonicMat_WorldTransform();
    DrawTitleModel(1);
}

void Zone_TitleScreen_4128A3() { DrawGameMenu(TitleMenu, 160, 184); }

void Zone_TitleScreen_4128CF() { DrawGameMenu(TitleMenu, 150, 48); }

void DrawMenuBackground()
{
    for (int32_t i = 0; i < TitleMenu.field_10 + 1; ++i) {
        if (TitleMenu.selMode[i] == 1)
            TitleMenu.field_14 = i;
    }

    SMenuY1 = 10 * TitleMenu.field_10;

    if (10 * TitleMenu.field_10 > SMenuY2 + 60 && SMenuY2 < 10 * TitleMenu.rowCount - 130)
        SMenuY2++;

    if (SMenuY1 < SMenuY2 + 60 && SMenuY1 > 0 && SMenuY2 > 0)
        SMenuY2--;

    DrawScrollingMenu(TitleMenu, 200, 48, 48, 160, -SMenuY2);
}

void Zone_TitleScreen_4129E4() { DrawGameMenu(TitleMenu, 160, 20); }

void ResetTitleScreen()
{
    TitleScreenMode = 0;
    TtlTime         = 0;
    SAnimation      = 0;
    TAnimation      = 0;
}