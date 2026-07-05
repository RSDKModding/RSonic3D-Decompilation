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
    InputData *input = &TitleInput;
    TextMenu *menu   = &TitleMenu;

    switch (TitleScreenMode) {
        case TITLE_TAXMAN_PRESENTS:
            CodeCheck();
            ClearScreen(0x000000);

            // Taxman presents!
            // Unfortunately no graphics are drawn here, but Retro-Sonic'd have this:
            // DrawSpriteNoKey(0, 0, 320, 240, 0, 0, 0);

            if (++TtlTime == 240) {
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
                    TextMeDo("NEW GAME", menu, 4, 0, 8);
                    SetGameText("CONTINUE GAME", menu, 1, 13);
                    SetGameText("DEBUG OPTIONS", menu, 2, 13);
                    SetGameText("EXIT GAME", menu, 3, 9);
                }
                else {
                    // In Retro-Sonic, this would show the same menu as above, with
                    // CUSTOM LEVELS instead of DEBUG OPTIONS. Seems like this path
                    // here is used as a player animation test instead
                    TextMeDo("STANDING", menu, 3, 0, 8);
                    SetGameText("WAITING", menu, 1, 7);
                    SetGameText("WALKING", menu, 2, 7);
                }

                menu->alignment      = MENU_ALIGN_CENTER;
                menu->selectionCount = 1;
                menu->selection1     = 0;
                SetFade(1.0f, 1.0f, 1.0f, 0.9f);
            }
            else {
                SetSceneRenderProperties(0);

                if (TtlTime < 61)
                    SetFade(0.0f, 0.0f, 0.0f, 1.0f - (TtlTime * 0.0166f));

                if (TtlTime > 179)
                    SetFade(1.0f, 1.0f, 1.0f, (TtlTime - 180) * 0.0166f);
            }
            break;

        case TITLE_LOGO_SCALE:
            TtlTime++;

            TitleScrMovement();
            SetSceneRenderProperties(1);

            DrawTitleScr();
            SetSceneRenderProperties(0);

            if (TtlTime < 61)
                SetFade(1.0f, 1.0f, 1.0f, 1.0f - (TtlTime * 0.0166f));

            if (TtlTime == 180) {
                TtlTime = 0;
                TitleScreenMode++;
            }
            break;

        case TITLE_MAIN:
            TitleScrMovement();

            SetSceneRenderProperties(1);
            DrawTitleScr();
            SetSceneRenderProperties(0);

            CheckInput(input);

            if (input->left == true)
                SAnimationSpeed -= 0.02f;

            if (input->right == true)
                SAnimationSpeed += 0.02f;

            if (SAnimationSpeed < 0.0f)
                SAnimationSpeed = 2 * RSDK_PI;

            if (2 * RSDK_PI < SAnimationSpeed)
                SAnimationSpeed = 0.0f;

            SetPlayerAnimation(menu->selection1 + 2, SAnimationSpeed);
            CheckKeyPress(input, INPUT_LEFT, INPUT_LCONTROL);

            if (input->down == true && menu->selection1 < menu->rowCount - 1)
                menu->selection1++;

            if (input->up == true && menu->selection1 > 0)
                menu->selection1--;

            if (input->start == true) {
                switch (menu->selection1) {
                    case OPTION_NEW_GAME:
                        LoadCharacterMenu(menu);
                        TitleScreenMode = TITLE_CHARACTER_MENU;
                        break;

                    case OPTION_CONTINUE_GAME: TitleScreenMode = TITLE_SAVE_MENU; break;

                    // Just a bit incorrect, eh
                    case OPTION_DEBUG_OPTIONS:
                        LoadCharacterMenu(menu);
                        TitleScreenMode = TITLE_CHARACTER_MENU;
                        break;

                    case OPTION_EXIT_GAME:
                        if (DebugEnabled == true) {
                            TextMeDo("DEBUG MENU", menu, 12, 0, 10);
                            SetGameText(" ", menu, 1, 1);
                            SetGameText(" ", menu, 2, 1);

                            if (AllStages)
                                SetGameText("ALL ZONES eONe ", menu, 3, 15);
                            else
                                SetGameText("ALL ZONES eOFFe", menu, 3, 15);

                            SetGameText(" ", menu, 4, 1);

                            if (DebugMode)
                                SetGameText("DEBUG MODE eONe ", menu, 5, 16);
                            else
                                SetGameText("DEBUG MODE eOFFe", menu, 5, 16);

                            SetGameText(" ", menu, 6, 1);
                            SetGameText("PLAY SPECIAL STAGE e00e", menu, 7, 23);

                            SetGameText(" ", menu, 8, 1);
                            SetGameText("SOUND TEST e00e", menu, 9, 15);

                            SetGameText(" ", menu, 10, 1);
                            SetGameText("eEXITe", menu, 11, 6);

                            menu->alignment      = MENU_ALIGN_CENTER;
                            menu->selectionCount = 2;
                            menu->selection1     = 0;
                            menu->selection2     = 3;

                            TitleScreenMode = TITLE_DEBUG_MENU;
                        }
                        break;

                    default: break;
                }
            }

            DrawGameMenu(TitleMenu, 160, 184);
            break;

        case TITLE_CHARACTER_MENU:
            ClearScreen(0x000000);
            SetSceneRenderProperties(1);
            SetSceneRenderProperties(0);

            CheckKeyPress(input, INPUT_UP, INPUT_LCONTROL);

            if (input->down == true && menu->selection1 < menu->rowCount - 1)
                menu->selection1++;

            if (input->up == true && menu->selection1 > 0)
                menu->selection1--;

            if (input->start == true) {
                LoadZoneMenu(menu);
                TitleScreenMode = TITLE_ZONE_MENU;
            }

            DrawGameMenu(TitleMenu, 150, 48);
            break;

        case TITLE_ZONE_MENU:
            ClearScreen(0x000000);
            SetSceneRenderProperties(1);
            SetSceneRenderProperties(0);

            CheckKeyPress(input, INPUT_UP, INPUT_LCONTROL);

            if (input->down == true && menu->selection1 < menu->rowCount - 1)
                menu->selection1++;

            if (input->up == true && menu->selection1 > 0)
                menu->selection1--;

            if (input->start == true) {
                TitleScreenMode = TITLE_ZONE_MENU;
                SetGameMode(GAMEMODE_MAINGAME);
            }

            DrawMenuBackground();
            break;

        case TITLE_DEBUG_MENU:
            ClearScreen(0x000000);
            SetSceneRenderProperties(1);
            SetSceneRenderProperties(0);

            CheckKeyPress(input, INPUT_LEFT, INPUT_LSHIFT);

            if (input->down == true && menu->selection2 < menu->rowCount - 1)
                menu->selection2 += 2;

            if (input->up == true && menu->selection2 > 3)
                menu->selection2 -= 2;

            if (input->left == true) {
                TextMenuEntry *entry = &menu->labels[menu->selection2];

                switch (menu->selection2) {
                    case 3:
                        if (AllStages) {
                            AllStages = false;
                            strcpy(&entry->text[12], "\x06\x06%");
                        }
                        else {
                            AllStages = true;
                            strcpy(&entry->text[12], "\x0E%");
                        }
                        break;

                    case 5:
                        if (DebugMode) {
                            DebugMode = false;
                            strcpy(&entry->text[13], "\x06\x06%");
                        }
                        else {
                            DebugMode = true;
                            strcpy(&entry->text[13], "\x0E%");
                        }
                        break;

                    case 7:
                        if (SStageNo > 0) {
                            entry->text[20] = --SStageNo / 10 + 27;
                            entry->text[21] = SStageNo + 27 - 10 * (SStageNo / 10);
                        }
                        break;

                    case 9:
                        if (MusicNo > 0) {
                            entry->text[12] = --MusicNo / 10 + 27;
                            entry->text[13] = MusicNo + 27 - 10 * (MusicNo / 10);
                        }
                        break;

                    default: break;
                }
            }

            if (input->right == true) {
                TextMenuEntry *entry = &menu->labels[menu->selection2];

                switch (menu->selection2) {
                    case 3:
                        if (AllStages) {
                            AllStages = false;
                            strcpy(&entry->text[12], "\x06%");
                        }
                        else {
                            AllStages = true;
                            strcpy(&entry->text[12], "\x0E%");
                        }
                        break;

                    case 5:
                        if (DebugMode) {
                            DebugMode = false;
                            strcpy(&entry->text[13], "\x06%");
                        }
                        else {
                            DebugMode = true;
                            strcpy(&entry->text[13], "\x0E%");
                        }
                        break;

                    case 7:
                        if (SStageNo < 99) {
                            entry->text[20] = ++SStageNo / 10 + 27;
                            entry->text[21] = SStageNo + 27 - 10 * (SStageNo / 10);
                        }
                        break;

                    case 9:
                        if (MusicNo < 99) {
                            entry->text[12] = ++MusicNo / 10 + 27;
                            entry->text[13] = MusicNo + 27 - 10 * (MusicNo / 10);
                        }
                        break;

                    default: break;
                }
            }

            if (input->start == true) {
                if (menu->selection2 == 11)
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

void SetMenuSelMode(TextMenu *menu, int id) { menu->selMode[id] = 1; }

void SetCharacters(int id)
{
    FileInfo file;
    LoadFile(&file, "Data/Title/Characters.mdf");

    char text[0x20];
    int textLen = 0;

    int row  = 0;
    int type = 0;

    bool twoPlayers = false;

    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '^') {
            switch (type) {
                case 0:
                    textLen = 0;
                    type++;
                    break;

                case 1:
                    twoPlayers = text[0] == '2';
                    textLen    = 0;
                    type++;
                    break;

                case 2:
                    if (row == id)
                        text[textLen] = '\0';

                    textLen = 0;
                    type++;
                    break;

                case 3:
                    if (twoPlayers && row == id)
                        text[textLen] = '\0';

                    textLen = 0;
                    type++;
                    break;

                default: break;
            }
        }
        else if (file.data[i] != '\r' && file.data[i] != '\n') {
            text[textLen++] = file.data[i];
        }

        if (file.data[i] == '\r') {
            row++;
            type    = 0;
            textLen = 0;
        }
    }

    delete file.data;
}

void CodeCheck()
{
    InputData *input = &TitleInput;
    CheckKeyPress(input, INPUT_LEFT, INPUT_START);

    if (input->up == true) {
        if (DebugCode[CodePos] == 2)
            CodePos++;
        else
            CodePos = 0;
    }

    if (input->down == true) {
        if (DebugCode[CodePos] == 3)
            CodePos++;
        else
            CodePos = 0;
    }

    if (input->left == true) {
        if (DebugCode[CodePos])
            CodePos = 0;
        else
            CodePos++;
    }

    if (input->right == true) {
        if (DebugCode[CodePos] == 1)
            CodePos++;
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

void DrawMenuBackground()
{
    TextMenu *menu = &TitleMenu;

    for (int i = 0; i < menu->selection1 + 1; ++i) {
        if (menu->selMode[i] == 1)
            menu->selection2 = i;
    }

    SMenuY1 = 10 * menu->selection1;

    if (10 * menu->selection1 > SMenuY2 + 60 && SMenuY2 < 10 * menu->rowCount - 130)
        SMenuY2++;

    if (SMenuY1 < SMenuY2 + 60 && SMenuY1 > 0 && SMenuY2 > 0)
        SMenuY2--;

    DrawScrollingMenu(TitleMenu, 200, 48, 48, 160, -SMenuY2);
}

void DrawTitleScr()
{
    ClearScreen(0x000000);

    BackXRotation += 0.0050f;
    if (2 * RSDK_PI < BackXRotation)
        BackXRotation = 0.0f;

    SetSceneRenderProperties(2);
    memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

    MatrixWorldRotateY(BackXRotation);
    MatrixMultiply(&MatrixObject, &MatrixWorld);

    MatrixWorldTranslateXYZ(0.0f, 0.0f, 20.0f);
    MatrixMultiply(&MatrixObject, &MatrixWorld);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
    DrawTitleModel(0);

    SetSceneRenderProperties(3);
    memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

    MatrixWorldTranslateXYZ(0.0f, 0.0f, SonZ);
    MatrixMultiply(&MatrixObject, &MatrixWorld);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
    DrawTitleModel(1);
}

void DrawGameMenu(TextMenu Menu, int x, int y)
{
    TextMenu *menu = &Menu;

    for (int i = 0; i < menu->rowCount; ++i) {
        TextMenuEntry *entry = &menu->labels[i];

        int drawX = x;
        int drawY = (10 * i) + y;

        switch (menu->alignment) {
            case MENU_ALIGN_RIGHT: drawX -= (10 * entry->length) / 1; break;
            case MENU_ALIGN_CENTER: drawX -= (10 * entry->length) / 2; break;
            default: break;
        }

        switch (menu->selectionCount) {
            case 1:
                if (i == menu->selection1)
                    DrawMenuText(entry->text, entry->length, drawX, drawY, 8);

                else
                    DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                break;

            case 2:
                if (i == menu->selection1 || i == menu->selection2)
                    DrawMenuText(entry->text, entry->length, drawX, drawY, 8);
                else
                    DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                break;

            case 3:
                if (menu->alignment == MENU_ALIGN_LEFT) {
                    if (i == menu->selection1)
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 8);

                    if (i != menu->selection1 && i == menu->selection2)
                        DrawMenuTextFaded(entry->text, entry->length, drawX, drawY);
                }
                break;

            default: break;
        }
    }
}

void DrawScrollingMenu(TextMenu Menu, int x, int y, int clipT, int clipB, int scrollPos)
{
    TextMenu *menu = &Menu;

    for (int i = 0; i < menu->rowCount; ++i) {
        TextMenuEntry *entry = &menu->labels[i];

        int drawX = x;
        int drawY = scrollPos + (10 * i) + y;

        switch (menu->alignment) {
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

        switch (menu->selectionCount) {
            case 1:
                if (i == menu->selection1) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawMenuTextClipped(entry->text, entry->length, drawX, drawY, 8, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 8);
                }
                else {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawMenuTextClipped(entry->text, entry->length, drawX, drawY, 0, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                }
                break;

            case 2:
                if (i == menu->selection1 || i == menu->selection2) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawMenuTextClipped(entry->text, entry->length, drawX, drawY, 8, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 8);
                }
                else {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawMenuTextClipped(entry->text, entry->length, drawX, drawY, 0, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                }
                break;

            case 3:
                if (i == menu->selection1) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawMenuTextClipped(entry->text, entry->length, drawX, drawY, 8, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 8);
                }
                else if (i != menu->selection2) {
                    if (drawY < clipT || drawY > clipB + 8)
                        DrawMenuTextClipped(entry->text, entry->length, drawX, drawY, 0, alphaStrengthT, alphaStrengthB);
                    else
                        DrawMenuText(entry->text, entry->length, drawX, drawY, 0);
                }

                if (i != menu->selection1 && i == menu->selection2)
                    DrawMenuTextFaded(entry->text, entry->length, drawX, drawY);
                break;

            default: break;
        }
    }
}

void DrawTitleModel(byte type)
{
    SetRenderMaterial(&RenderMaterial);

    switch (type) {
        case 0: {
            TMF *model = &BGModel;
            SetRenderTexture(0, BGTexture);
            DrawFace(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);
            break;
        }

        case 1: {
            TMF *model = &LogoModel;
            SetRenderTexture(0, LogoTexture);
            DrawFace(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);
            break;
        }

        default: break;
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

    int row  = 0;
    int type = 0;

    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '^') {
            text[textLen] = '\0';

            switch (type) {
                case 0:
                    SetGameText(text, menu, row, textLen);
                    textLen = 0;
                    type++;
                    break;

                case 1:
                    textLen = 0;
                    type++;
                    break;

                case 2:
                    textLen = 0;
                    type++;
                    break;

                case 3:
                    textLen = 0;
                    type++;
                    break;

                default: break;
            }
        }
        else if (file.data[i] != '\r' && file.data[i] != '\n') {
            text[textLen] = file.data[i];
            textLen++;
        }

        if (file.data[i] == '\r') {
            row++;
            type    = 0;
            textLen = 0;
        }
    }

    menu->alignment      = MENU_ALIGN_LEFT;
    menu->selectionCount = 1;
    menu->selection1     = 0;
    menu->selection2     = 0;
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

    int row  = 0;
    int type = 0;

    for (int i = 0; i < file.size; ++i) {
        if (file.data[i] == '^') {
            text[textLen] = '\0';

            switch (type) {
                case 0: SetGameText(text, menu, row, textLen); break;
                case 1: SetLevelDirectory(text, textLen, row); break;
                case 2: SetActNumber(text, textLen, row); break;

                case 3:
                    if (text[0] == '1')
                        SetMenuSelMode(menu, row);
                    break;

                default: break;
            }

            textLen = 0;
            type++;
        }
        else if (file.data[i] != '\r' && file.data[i] != '\n') {
            if (textLen < 0x20 - 1) {
                text[textLen] = file.data[i];
                textLen++;
            }
        }

        if (file.data[i] == '\r') {
            row++;
            type    = 0;
            textLen = 0;
        }
    }

    menu->alignment      = MENU_ALIGN_RIGHT;
    menu->selectionCount = 3;
    menu->selection1     = 0;
    menu->selection2     = 0;
}