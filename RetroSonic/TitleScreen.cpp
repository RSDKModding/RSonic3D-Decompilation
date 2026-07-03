#include "RetroEngine.hpp"

InputData TitleInput;
TextMenu TitleMenu;

float BackXRotation;
float backpos[10];

float SonX     = 0.0f;
float SonY     = 0.0f;
float SonZ     = 70.0f;
float SXspeed  = 0.0f;
float SYspeed  = 0.0f;
int SAnimation = 0;
float SAnimationSpeed;

float TailsX   = 0.0f;
float TailsY   = 0.0f;
float TailsZ   = 0.0f;
float TXspeed  = 0.0f;
float TYspeed  = 0.0f;
int TAnimation = 0;
float TAnimationSpeed;

int TitleScreenMode;
int TtlTime;

char TxtStyle;

float RysS;
float SwS;

int AllStages;
byte CodePos;

int SMenuY1;
int SMenuY2;

int MusicNo;
int SStageNo;

int DebugEnabled = true;
int DebugMode    = true;

byte DebugCode[12] = { 2, 2, 3, 3, 0, 1, 0, 1, 4, 0, 0, 0 };

void ResetTitleScreen()
{
    TitleScreenMode = 0;
    TtlTime         = 0;

    SAnimation = 0;
    TAnimation = 0;
}

void ProcessTitleScreen()
{
    switch (TitleScreenMode) {
        case 0:
            ++TtlTime;
            CodeCheck();
            ClearScreen(0x000000);

            if (TtlTime == 240) {
                TtlTime = 0;
                ++TitleScreenMode;

                backpos[0] = -0.5f;
                backpos[1] = -1.0f;
                backpos[2] = -0.1f;
                backpos[3] = -0.4f;
                backpos[4] = -0.7f;
                backpos[5] = -0.2f;
                backpos[6] = -0.4f;
                backpos[7] = -0.3f;
                backpos[8] = -0.8f;
                backpos[9] = -1.0f;

                SonX       = 130.0f;
                SonY       = 142.0f;
                SXspeed    = 0.0f;
                SYspeed    = -2.0f;
                SAnimation = 0;

                TailsX     = 100.0f;
                TailsY     = 146.0f;
                TXspeed    = 0.0f;
                TYspeed    = -2.0f;
                TAnimation = 0;

                SMenuY1 = 0;
                SMenuY2 = 0;

                RysS = 80.0f;
                SwS  = 8.0f;

                TxtStyle = 0;

                if (DebugEnabled == true) {
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

                TitleMenu.alignment      = MENU_ALIGN_CENTER;
                TitleMenu.selectionCount = 1;
                TitleMenu.selection1     = 0;
                SetFade(1.0f, 1.0f, 1.0f, 0.9f);
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

            if (TitleInput.left == true)
                SAnimationSpeed -= 0.02f;

            if (TitleInput.right == true)
                SAnimationSpeed += 0.02f;

            if (SAnimationSpeed < 0.0f)
                SAnimationSpeed = 2 * RSDK_PI;

            if (2 * RSDK_PI < SAnimationSpeed)
                SAnimationSpeed = 0.0f;

            SetPlayerAnimationID(TitleMenu.selection1 + 2, SAnimationSpeed);

            CheckKeyPress(&TitleInput, INPUT_LEFT, INPUT_LCONTROL);

            if (TitleInput.down == true && TitleMenu.selection1 < TitleMenu.rowCount - 1)
                TitleMenu.selection1++;

            if (TitleInput.up == true && TitleMenu.selection1 > 0)
                TitleMenu.selection1--;

            if (TitleInput.start == true) {
                switch (TitleMenu.selection1) {
                    case 0:
                        LoadCharacterMenu(&TitleMenu);
                        TitleScreenMode = 3;
                        break;

                    case 1: TitleScreenMode = 5; break;

                    case 2:
                        LoadCharacterMenu(&TitleMenu);
                        TitleScreenMode = 3;
                        break;

                    case 3:
                        if (DebugEnabled == true) {
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

                            TitleMenu.alignment      = MENU_ALIGN_CENTER;
                            TitleMenu.selectionCount = 2;
                            TitleMenu.selection1     = 0;
                            TitleMenu.selection2     = 3;

                            TitleScreenMode = 6;
                        }
                        break;

                    default: break;
                }
            }

            DrawGameMenu(TitleMenu, 160, 184);
            break;

        case 3:
            ClearScreen(0x000000);
            DrawTitleScr(1);
            DrawTitleScr(0);

            CheckKeyPress(&TitleInput, INPUT_UP, INPUT_LCONTROL);

            if (TitleInput.down == true && TitleMenu.selection1 < TitleMenu.rowCount - 1)
                TitleMenu.selection1++;

            if (TitleInput.up == true && TitleMenu.selection1 > 0)
                TitleMenu.selection1--;

            if (TitleInput.start == true) {
                LoadZoneMenu(&TitleMenu);
                TitleScreenMode = 4;
            }

            DrawGameMenu(TitleMenu, 150, 48);
            break;

        case 4:
            ClearScreen(0x000000);
            DrawTitleScr(1);
            DrawTitleScr(0);

            CheckKeyPress(&TitleInput, INPUT_UP, INPUT_LCONTROL);

            if (TitleInput.down == true && TitleMenu.selection1 < TitleMenu.rowCount - 1)
                ++TitleMenu.selection1;

            if (TitleInput.up == true && TitleMenu.selection1 > 0)
                --TitleMenu.selection1;

            if (TitleInput.start == true) {
                TitleScreenMode = 4;
                SetGameMode(GAMEMODE_MAINGAME);
            }

            DrawMenuBackground();
            break;

        case 6:
            ClearScreen(0x000000);
            DrawTitleScr(1);
            DrawTitleScr(0);

            CheckKeyPress(&TitleInput, INPUT_LEFT, INPUT_LSHIFT);

            if (TitleInput.down == true && TitleMenu.selection2 < TitleMenu.rowCount - 1)
                TitleMenu.selection2 += 2;

            if (TitleInput.up == true && TitleMenu.selection2 > 3)
                TitleMenu.selection2 -= 2;

            if (TitleInput.left == true) {
                switch (TitleMenu.selection2) {
                    case 3:
                        if (AllStages) {
                            AllStages = false;
                            strcpy(&TitleMenu.labels[3].text[12], "\x06\x06%");
                        }
                        else {
                            AllStages = true;
                            strcpy(&TitleMenu.labels[3].text[12], "\x0E%");
                        }
                        break;

                    case 5:
                        if (DebugMode) {
                            DebugMode = false;
                            strcpy(&TitleMenu.labels[5].text[13], "\x06\x06%");
                        }
                        else {
                            DebugMode = true;
                            strcpy(&TitleMenu.labels[5].text[13], "\x0E%");
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

            if (TitleInput.right == true) {
                switch (TitleMenu.selection2) {
                    case 3:
                        if (AllStages) {
                            AllStages = 0;
                            strcpy(&TitleMenu.labels[3].text[12], "\x06%");
                        }
                        else {
                            AllStages = 1;
                            strcpy(&TitleMenu.labels[3].text[12], "\x0E%");
                        }
                        break;

                    case 5:
                        if (DebugMode) {
                            DebugMode = false;
                            strcpy(&TitleMenu.labels[5].text[13], "\x06%");
                        }
                        else {
                            DebugMode = true;
                            strcpy(&TitleMenu.labels[5].text[13], "\x0E%");
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

            if (TitleInput.start == true) {
                if (TitleMenu.selection2 == 11)
                    TitleScreenMode = 0;
            }

            DrawGameMenu(TitleMenu, 160, 20);
            break;

        default: break;
    }

    FlipScreen();
}

void TitleScrMovement()
{
    if (TtlTime > 30 && SonX < 160.0f) {
        SXspeed = SXspeed + 0.0099999998f;
        SYspeed = SYspeed + 0.02f;
        SonX    = SonX + SXspeed;
        SonY    = SonY + SYspeed;
        SonZ    = SYspeed * 0.5f + SonZ;
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

void DrawMenuBackground()
{
    for (int i = 0; i < TitleMenu.selection1 + 1; ++i) {
        if (TitleMenu.selMode[i] == 1)
            TitleMenu.selection2 = i;
    }

    SMenuY1 = 10 * TitleMenu.selection1;

    if (10 * TitleMenu.selection1 > SMenuY2 + 60 && SMenuY2 < 10 * TitleMenu.rowCount - 130)
        SMenuY2++;

    if (SMenuY1 < SMenuY2 + 60 && SMenuY1 > 0 && SMenuY2 > 0)
        SMenuY2--;

    DrawScrollingMenu(TitleMenu, 200, 48, 48, 160, -SMenuY2);
}

void DrawTitleScr(sbyte id)
{
    switch (id) {
        case 0:
            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixIdentity);
            SetRenderTransform(RENDER_TRANSFORM_VIEW, &MatrixIdentity);
            SetRenderTransform(RENDER_TRANSFORM_PROJECTION, &MatrixIdentity);

            SetRenderState(RENDER_STATE_ZENABLE, false);
            SetRenderState(RENDER_STATE_LIGHTING, false);
            SetRenderState(RENDER_STATE_SPECULARENABLE, false);
            break;

        case 1:
            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixIdentity);
            SetRenderTransform(RENDER_TRANSFORM_VIEW, &MatrixView);
            SetRenderTransform(RENDER_TRANSFORM_PROJECTION, &MatrixProjection);

            SetRenderState(RENDER_STATE_ZENABLE, true);
            SetRenderState(RENDER_STATE_LIGHTING, true);
            SetRenderState(RENDER_STATE_SPECULARENABLE, false);
            break;

        case 2: SetRenderState(RENDER_STATE_ZENABLE, false); break;
        case 3: SetRenderState(RENDER_STATE_ZENABLE, true); break;

        default: break;
    }
}

void DrawGameMenu(TextMenu TextMenu, int x, int y)
{
    for (int i = 0; i < TextMenu.rowCount; ++i) {
        TextMenuEntry *entry = &TextMenu.labels[i];

        int xdraw = x;
        int ydraw = (10 * i) + y;

        switch (TextMenu.alignment) {
            case MENU_ALIGN_RIGHT: xdraw -= (10 * entry->length) / 1; break;
            case MENU_ALIGN_CENTER: xdraw -= (10 * entry->length) / 2; break;
            default: break;
        }

        switch (TextMenu.selectionCount) {
            case 1:
                if (i == TextMenu.selection1)
                    DrawMenuText(entry->text, entry->length, xdraw, ydraw, 8);

                else
                    DrawMenuText(entry->text, entry->length, xdraw, ydraw, 0);
                break;

            case 2:
                if (i == TextMenu.selection1 || i == TextMenu.selection2)
                    DrawMenuText(entry->text, entry->length, xdraw, ydraw, 8);
                else
                    DrawMenuText(entry->text, entry->length, xdraw, ydraw, 0);
                break;

            case 3:
                if (TextMenu.alignment == MENU_ALIGN_LEFT) {
                    if (i == TextMenu.selection1)
                        DrawMenuText(entry->text, entry->length, xdraw, ydraw, 8);

                    if (i != TextMenu.selection1 && i == TextMenu.selection2)
                        DrawText_2(entry->text, entry->length, xdraw, ydraw);
                }
                break;

            default: break;
        }
    }
}

void DrawScrollingMenu(TextMenu TextMenu, int x, int y, int clipT, int clipB, int scrollPos)
{
    for (int i = 0; i < TextMenu.rowCount; ++i) {
        TextMenuEntry *entry = &TextMenu.labels[i];

        int drawX = x;
        int drawY = scrollPos + (10 * i) + y;

        switch (TextMenu.alignment) {
            case MENU_ALIGN_RIGHT: drawX -= (10 * entry->length) / 1; break;
            case MENU_ALIGN_CENTER: drawX -= (10 * entry->length) / 2; break;
            default: break;
        }

        int alphaStrengthT = 0;
        int alphaStrengthB = 0;

        if (drawY < clipT)
            alphaStrengthT = clipT - drawY;

        if (drawY > clipB + 8)
            alphaStrengthB = drawY - (clipB + 8);

        switch (TextMenu.selectionCount) {
            case 1:
                if (i == TextMenu.selection1) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawText_3(entry->text, entry->length, drawX, drawY, 8, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 8);
                }
                else {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawText_3(entry->text, entry->length, drawX, drawY, 0, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                }
                break;

            case 2:
                if (i == TextMenu.selection1 || i == TextMenu.selection2) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawText_3(entry->text, entry->length, drawX, drawY, 8, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 8);
                }
                else {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawText_3(entry->text, entry->length, drawX, drawY, 0, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                }
                break;

            case 3:
                if (i == TextMenu.selection1) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawText_3(entry->text, entry->length, drawX, drawY, 8, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 8);
                }
                else if (i != TextMenu.selection2) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawText_3(entry->text, entry->length, drawX, drawY, 0, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                }

                if (i != TextMenu.selection1 && i == TextMenu.selection2)
                    DrawText_2(entry->text, entry->length, drawX, drawY);
                break;

            default: break;
        }
    }
}

void LoadCharacterMenu(TextMenu *menu)
{
    FileInfo file;
    LoadFile(&file, "Data/Title/Characters.mdf");

    int rowCount = 0;
    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '\n')
            rowCount++;
    }

    TextMeDo("ZONES", menu, rowCount, 0, 5);
    AllocateDirectories(rowCount);

    char text[0x20];
    int textLen = 0;

    int menuRow = 0;
    int rowType = 0;

    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '^') {
            text[textLen] = '\0';

            switch (rowType) {
                case 0:
                    SetGameText(text, menu, menuRow, textLen);
                    textLen = 0;
                    rowType++;
                    break;

                case 1:
                    textLen = 0;
                    rowType++;
                    break;

                case 2:
                    textLen = 0;
                    rowType++;
                    break;

                case 3:
                    textLen = 0;
                    rowType++;
                    break;

                default: break;
            }
        }
        else if (file.data[i] != '\r' && file.data[i] != '\n') {
            text[textLen] = file.data[i];
            textLen++;
        }

        if (file.data[i] == '\r') {
            menuRow++;
            rowType = 0;
            textLen = 0;
        }
    }

    menu->alignment      = MENU_ALIGN_LEFT;
    menu->selectionCount = 1;
    menu->selection1     = 0;
    menu->selection2     = 0;
}

void SetMenuSelMode(TextMenu *menu, int id) { menu->selMode[id] = 1; }

void SetCharacters(int row)
{
    FileInfo file;
    LoadFile(&file, "Data/Title/Characters.mdf");

    char text[0x20];
    int textLen = 0;

    int menuRow = 0;
    int rowType = 0;

    bool unknown;

    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '^') {
            switch (rowType) {
                case 0:
                    textLen = 0;
                    ++rowType;
                    break;

                case 1:
                    unknown = text[0] == '2';
                    textLen = 0;
                    ++rowType;
                    break;

                case 2:
                    if (menuRow == row)
                        text[textLen] = '\0';

                    textLen = 0;
                    ++rowType;
                    break;

                case 3:
                    if (unknown && menuRow == row)
                        text[textLen] = '\0';

                    textLen = 0;
                    ++rowType;
                    break;

                default: break;
            }
        }
        else if (file.data[i] != '\r' && file.data[i] != '\n') {
            text[textLen++] = file.data[i];
        }

        if (file.data[i] == '\r') {
            ++menuRow;
            rowType = 0;
            textLen = 0;
        }
    }

    delete file.data;
}

void CodeCheck()
{
    CheckKeyPress(&TitleInput, INPUT_LEFT, INPUT_START);

    if (TitleInput.up == true) {
        if (DebugCode[CodePos] == 2)
            ++CodePos;
        else
            CodePos = 0;
    }

    if (TitleInput.down == true) {
        if (DebugCode[CodePos] == 3)
            ++CodePos;
        else
            CodePos = 0;
    }

    if (TitleInput.left == true) {
        if (DebugCode[CodePos])
            CodePos = 0;
        else
            ++CodePos;
    }

    if (TitleInput.right == true) {
        if (DebugCode[CodePos] == 1)
            ++CodePos;
        else
            CodePos = 0;
    }

    if (DebugCode[CodePos] == 4) {
        if (DebugEnabled == true)
            DebugEnabled = false;
        else if (DebugEnabled == false)
            DebugEnabled = true;

        CodePos = 0;
    }
}

void LoadZoneMenu(TextMenu *menu)
{
    FileInfo file;
    LoadFile(&file, "Data/Title/Zones.mdf");

    int rowCount = 0;
    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '\n')
            rowCount++;
    }

    TextMeDo("ZONES", menu, rowCount, 0, 5);
    AllocateDirectories(rowCount);

    char text[0x20];
    int textLen = 0;

    int menuRow = 0;
    int rowType = 0;

    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '^') {
            text[textLen] = '\0';

            switch (rowType) {
                case 0: SetGameText(text, menu, menuRow, textLen); break;
                case 1: SetLevelDirectory(text, textLen, menuRow); break;
                case 2: SetActNumber(text, textLen, menuRow); break;

                case 3:
                    if (text[0] == '1')
                        SetMenuSelMode(menu, menuRow);
                    break;

                default: break;
            }

            textLen = 0;
            rowType++;
        }
        else if (file.data[i] != '\r' && file.data[i] != '\n') {
            if (textLen < 0x20 - 1) {
                text[textLen] = file.data[i];
                textLen++;
            }
        }

        if (file.data[i] == '\r') {
            menuRow++;
            rowType = 0;
            textLen = 0;
        }
    }

    menu->alignment      = MENU_ALIGN_RIGHT;
    menu->selectionCount = 3;
    menu->selection1     = 0;
    menu->selection2     = 0;
}

void Zone_TitleScreen_4127E6()
{
    ClearScreen(0x000000);

    BackXRotation += 0.0050f;
    if (2 * RSDK_PI < BackXRotation)
        BackXRotation = 0.0f;

    DrawTitleScr(2);
    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    WorldMatrixRotateY(BackXRotation);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixTranslateXYZ(0.0f, 0.0f, 20.0f);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
    DrawTitleModel(0);

    DrawTitleScr(3);
    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    WorldMatrixTranslateXYZ(0.0f, 0.0f, SonZ);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
    DrawTitleModel(1);
}

void DrawTitleModel(char type)
{
    SetRenderMaterial(&ObjectMaterial);

    if (type == 1) {
        SetRenderTexture(0, LogoTexture);
        DrawIndexedPrimitive(RENDER_FVF_VERTEX, LogoModel.vertices, LogoModel.numVertices, LogoModel.indices, LogoModel.numIndices);
    }
    else if (type == 0) {
        SetRenderTexture(0, BGTexture);
        DrawIndexedPrimitive(RENDER_FVF_VERTEX, BGModel.vertices, BGModel.numVertices, BGModel.indices, BGModel.numIndices);
    }
}