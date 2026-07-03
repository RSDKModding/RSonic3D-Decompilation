#include "RetroEngine.hpp"

unsigned long long Frequency;
unsigned long long FrameCurrentTicks;
unsigned long long FrameNextTicks;
unsigned long long FrameLastTicks;

int FrameTicks = 16;
double FrameSecondsPerTick;
double FrameDeltaTime;

#if RETRO_USE_ORIGINAL_CODE
MSG Message;
bool UseQueryCounter;
#endif

int QuitMessage = EXIT_FAILURE;

const char *ResolutionOptions[] = { "320x240", "640x480", "800x600", "1024x768" };

bool CreateMWindow()
{
#if RETRO_USE_ORIGINAL_CODE
    const char *title = "Retro-Sonic 3D: A Taxman Test";

    WNDCLASSA wndClass;
    wndClass.style         = CS_VREDRAW | CS_HREDRAW;
    wndClass.lpfnWndProc   = MWindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 4;
    wndClass.hInstance     = HInst;
    wndClass.hIcon         = LoadIconA(HInst, MAKEINTRESOURCEA(0x65));
    wndClass.hCursor       = LoadCursorA(0, MAKEINTRESOURCEA(0x7F00));
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = (LPCSTR)IDR_MAINMENU;
    wndClass.lpszClassName = title;

    if (RegisterClassA(&wndClass)) {
        int mtcapt = GetSystemMetrics(SM_CYCAPTION);
        int mtmenu = GetSystemMetrics(SM_CYMENU);

        int w = (2 * GetSystemMetrics(SM_CXSIZEFRAME)) + (SCREEN_XSIZE * 2);
        int h = (2 * GetSystemMetrics(SM_CYSIZEFRAME)) + (SCREEN_YSIZE * 2);

        int style = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

        HWnd = CreateWindow(title, title, style, CW_USEDEFAULT, CW_USEDEFAULT, w, (mtmenu + mtcapt) + h, 0, 0, HInst, 0);
        if (HWnd != NULL) {
            ShowWindow(HWnd, NCmdShow);
            UpdateWindow(HWnd);
            EngineRunning = true;
            return InitInputDevice() && InitGraphicsAPI();
        }
    }
    else {
        MessageBox(HWnd, "Error Registering Window Class", 0, MB_ICONHAND);
    }

    return false;
#else
    SetScreenResolution(SCALE_2X, SCALE_2X, 0);

#if RETRO_USE_SDL3
    Uint32 flags = SDL_WINDOW_OPENGL;
#elif RETRO_USE_SDL2
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
#endif
    if (WindowMode == 0)
        flags |= SDL_WINDOW_FULLSCREEN;

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

#if RETRO_USE_SDL3
    Window = SDL_CreateWindow("Retro-Sonic 3D: A Taxman Test", ResX, ResY, flags);
#elif RETRO_USE_SDL2
    Window = SDL_CreateWindow("Retro-Sonic 3D: A Taxman Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ResX, ResY, flags);
#endif
    if (!Window)
        return false;

    GLContext = SDL_GL_CreateContext(Window);
    if (!GLContext)
        return false;

    EnableVSync(true);

    EngineRunning = true;
    return InitInputDevice() && InitGraphicsAPI();
#endif
}

#if RETRO_USE_ORIGINAL_CODE
LRESULT CALLBACK MWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
        case WM_DESTROY:
            QuitMessage = EXIT_SUCCESS;
            PostQuitMessage(EXIT_SUCCESS);
            return 0;

        case WM_MOVE:
        case WM_SIZE: UpdateWindowRect(hWnd); return 0;

        case WM_ACTIVATE:
            if (wParam)
                EnableInput();
            else
                DisableInput();

            return 0;

        case WM_KEYDOWN:
            switch (wParam) {
                case VK_ESCAPE:
                    QuitMessage = EXIT_SUCCESS;
                    PostQuitMessage(EXIT_SUCCESS);
                    break;

                case VK_F2:
                    GameMode = GAMEMODE_TITLESCREEN;
                    ResetTitleScreen();

                    for (int i = 0; i < 6; ++i) {
                        ReleaseCharacterUITexture(i);
                    }

                    LoadFontAssets();
                    break;

                case VK_F4:
                    ResetWindow(hWnd);
                    if (GameMode == GAMEMODE_TITLESCREEN)
                        LoadFontAssets();
                    break;

                case VK_F5: DialogBoxParamA(HInst, "DMSelect", hWnd, MDialogProc, 0); break;
            }

            return DefWindowProcA(hWnd, Msg, wParam, lParam);

        case WM_COMMAND:
            switch (wParam) {
                case IDM_GAME_RESET:
                    GameMode = GAMEMODE_TITLESCREEN;
                    ResetTitleScreen();
                    break;

                case IDM_GAME_QUIT:
                    QuitMessage = EXIT_SUCCESS;
                    PostQuitMessage(EXIT_SUCCESS);
                    break;

                case IDM_HELP_ABOUT: break;

                case IDM_SET_DISPLAY: DialogBoxParamA(HInst, "DMSelect", hWnd, MDialogProc, 0); break;

                case IDM_OPT_FULLSCREEN:
                    ResetWindow(hWnd);
                    if (GameMode == GAMEMODE_TITLESCREEN)
                        LoadFontAssets();
                    break;

                case IDM_ERASE_SAVE: break;
            }

            return 0;

        default: return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
}

INT_PTR CALLBACK MDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
        case WM_INITDIALOG:
            for (int i = 0; i < 4; ++i) {
                SendDlgItemMessageA(hWnd, IDC_DM_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)ResolutionOptions[i]);
            }

            for (int i = 0; i < 2; ++i) {
                SendDlgItemMessageA(hWnd, IDC_DM_WINDOW, CB_ADDSTRING, 0, (LPARAM)ResolutionOptions[i]);
            }

            SendDlgItemMessageA(hWnd, IDC_DM_RESOLUTION, CB_SETCURSEL, 0, 0);
            SendDlgItemMessageA(hWnd, IDC_DM_WINDOW, CB_SETCURSEL, 0, 0);
            SendDlgItemMessageA(hWnd, IDC_DM_STRETCH, BM_SETCHECK, BST_CHECKED, 0);
            return 1;

        case WM_COMMAND:
            if (wParam == IDC_DM_OK) {
                char fullscreen = SendDlgItemMessageA(hWnd, IDC_DM_RESOLUTION, CB_GETCURSEL, 0, 0);
                char window     = SendDlgItemMessageA(hWnd, IDC_DM_RESOLUTION, CB_GETCURSEL, 0, 0);

                char centerScreen = SendDlgItemMessageA(hWnd, IDC_DM_CENTER, BM_GETCHECK, BST_CHECKED, 0);
                char stretchFit   = SendDlgItemMessageA(hWnd, IDC_DM_STRETCH, BM_GETCHECK, BST_CHECKED, 0);
                char sai2X        = SendDlgItemMessageA(hWnd, IDC_DM_2XSAI, BM_GETCHECK, BST_CHECKED, 0);

                if (centerScreen == BST_CHECKED)
                    SetScreenResolution(fullscreen, window, 1);

                if (stretchFit == BST_CHECKED)
                    SetScreenResolution(fullscreen, window, 0);

                if (sai2X == BST_CHECKED)
                    SetScreenResolution(fullscreen, window, 2);
            }

            if (wParam == IDC_DM_OK || wParam == IDC_DM_CANCEL) {
                EndDialog(hWnd, 1);
                return 1;
            }

            return 0;

        case WM_CLOSE: EndDialog(hWnd, 1); return 1;

        default: return 0;
    }
}

void UpdateWindowRect(HWND hWnd)
{
    if (WindowMode == 1) {
        GetClientRect(hWnd, &clientRect);
        GetClientRect(hWnd, &windowBounds);
        ClientToScreen(hWnd, (POINT *)&windowBounds.left);
        ClientToScreen(hWnd, (POINT *)&windowBounds.right);
    }
}

void ResetWindow(HWND hWnd)
{
    if (WindowMode == 1)
        GetWindowRect(hWnd, &rect_420488);

    ToggleScreenMode();
}
#else
void ProcessEvents(SDL_Event &event)
{
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
#if RETRO_USE_SDL3
            case SDL_EVENT_QUIT:
#elif RETRO_USE_SDL2
            case SDL_QUIT:
#endif
                EngineRunning = false;
                break;

#if RETRO_USE_SDL3
            case SDL_EVENT_WINDOW_FOCUS_GAINED: EnableInput(); break;

            case SDL_EVENT_WINDOW_FOCUS_LOST: DisableInput(); break;
#elif RETRO_USE_SDL2
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                    EnableInput();

                if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                    DisableInput();
                break;
#endif

#if RETRO_USE_SDL3
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
#elif RETRO_USE_SDL2
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
#endif

                    case SDLK_ESCAPE: EngineRunning = false; break;

                    case SDLK_F2:
                        GameMode = GAMEMODE_TITLESCREEN;
                        ResetTitleScreen();

                        for (int i = 0; i < 6; ++i) {
                            ReleaseCharacterUITexture(i);
                        }

                        LoadFontAssets();
                        break;

                    case SDLK_F4:
                        ToggleScreenMode();
                        if (GameMode == GAMEMODE_TITLESCREEN)
                            LoadFontAssets();
                        break;

                    default: break;
                }
                break;

            default: break;
        }
    }
}
#endif

void ProcessMainGame()
{
    switch (MainGameMode) {
        case 0:
            data_4C9F68.y = 10.0f;
            data_4C9F68.z = -32.0f;
            MainGameMode  = 2;

            // x-axis line of 6 rings
            for (ObjectLoop = 0; ObjectLoop < 6; ++ObjectLoop) {
                Object *ring = &LevelObjects[ObjectLoop];

                ring->type       = OBJECT_RING;
                ring->enabled    = true;
                ring->position.x = (double)ObjectLoop * -8.0 - 10.0;
                ring->position.z = -20.0f;
                ring->position.y = 5.0f;
            }

            // z-axis line of 4 rings
            for (ObjectLoop = 10; ObjectLoop < 14; ++ObjectLoop) {
                Object *ring = &LevelObjects[ObjectLoop];

                ring->type       = OBJECT_RING;
                ring->enabled    = true;
                ring->position.z = (double)ObjectLoop * 8.0f - 60.0f;
                ring->position.x = -50.0f;
                ring->position.y = 12.0f;
            }

            // 1-element for loop that spawns the spring object, lol
            for (ObjectLoop = 14; ObjectLoop < 15; ++ObjectLoop) {
                Object *spring = &LevelObjects[ObjectLoop];

                spring->type       = OBJECT_SPRING;
                spring->enabled    = true;
                spring->position.z = 50.0f;
                spring->position.x = -80.0f;
                spring->position.y = 1.5f;
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
            FlipScreen();
            break;

        case 10:
        case 11: FlipScreen(); break;

        default: break;
    }
}

void SetGameMode(int mode) { GameMode = mode; }

void PauseCheck()
{
    if (MGameInput.start == true) {
        if (PauseV) {
            if (PauseV == 1)
                PauseV = 0;
        }
        else {
            PauseV = 1;
        }
    }
}

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

    v18 = 0;

    Player[PNumber].up        = false;
    Player[PNumber].jumpPress = false;

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
                Player[PNumber].up = true;
                Player[PNumber].f_0x2C /= v18;
            }

            Player[PNumber].f_0x50 = MGameInput.Z == 1;
            CheckKeyPress(&MGameInput, INPUT_START, INPUT_Z);
        }

        if (MGameInput.control == true)
            Player[PNumber].jumpPress = true;

        if (MGameInput.shift == true && DebugEn == true)
            Debug ^= true;
    }

    CheckInput(&MGameInput);
}

void ProcessDebugMode()
{
    // chillin
}

void ProcessPlayerMovement()
{
    PlayerObject *player = &Player[PNumber];

    float a6; // [esp+0h] [ebp-14h]
    float v1; // [esp+4h] [ebp-10h]
    float v2; // [esp+4h] [ebp-10h]
    char v3;  // [esp+8h] [ebp-Ch]

    if (player->f_0x38 == 1 && player->up == true) {
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
    else if (player->f_0x38 == 0 && player->up == true) {
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
        if (player->gravity == GRAVITY_AIR) {
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

            if (player->jumpPress == true) {
                player->gravity    = GRAVITY_AIR;
                player->velocity.y = 2.0;
                player->f_0x38     = 1;
                ResetPlayerRotation();
            }
        }
    }

    player->velocity.x   = -Sin(player->f_0x30) * player->f_0x24;
    player->velocity.z   = Cos(player->f_0x30) * player->f_0x24;
    player->collideDir.x = 0.0;
    player->collideDir.y = -4.0;
    player->collideDir.z = 0.0;

    if (player->gravity == GRAVITY_AIR) {
        if (player->velocity.y < 0.0) {
            a6 = player->velocity.y * -1.0;
            if (ObjectFloorCollision(&player->position.x, &player->position.y, &player->position.z, player->velocity.x, a6, player->velocity.z)
                == 1) {
                player->velocity.y = 0.0;
                player->gravity    = GRAVITY_GROUND;
                player->f_0x38     = 0;
            }
        }
    }
    else {
        player->velocity.y = 0.0;
        PlayerRotationPhysics(&player->velocity.x, &player->velocity.y, &player->velocity.z);
        player->collideDir.x = 0.0;
        player->collideDir.y = -3.5;
        player->collideDir.z = 0.0;

        PlayerRotationPhysics(&player->collideDir.x, &player->collideDir.y, &player->collideDir.z);

        player->position.x = player->position.x - (player->collideDir.x - player->collideDir.x * 0.25);
        player->position.y = player->position.y - (player->collideDir.y - player->collideDir.y * 0.25);
        player->position.z = player->position.z - (player->collideDir.z - player->collideDir.z * 0.25);

        v3 = ObjectFloorCollision(&player->position.x, &player->position.y, &player->position.z, player->collideDir.x, player->collideDir.y,
                                  player->collideDir.z);
        if (v3) {
            if (v3 > 0 && v3 <= 2) {
                player->gravity = GRAVITY_GROUND;
                player->f_0x38  = 0;
            }
        }
        else {
            player->position.x = player->collideDir.x - player->collideDir.x * 0.25 + player->position.x;
            player->position.y = player->collideDir.y - player->collideDir.y * 0.25 + player->position.y;
            player->position.z = player->collideDir.z - player->collideDir.z * 0.25 + player->position.z;
            player->gravity    = GRAVITY_AIR;
        }
    }

    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y;
    player->position.z += player->velocity.z;
    ProcessPlayerCamera();

    if (player->up == true)
        player->f_0x30 = (double)player->f_0x28 / 128.0 * 3.1415927 + data_4C9F74;

    if (player->gravity == GRAVITY_AIR) {
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

void ProcessObjects()
{
    for (ObjectLoop = 0; ObjectLoop < 1100; ++ObjectLoop) {
        Object *object = &LevelObjects[ObjectLoop];
        if (object->enabled > 0) {
            switch (object->type) {
                case OBJECT_RING: {
                    Vector3D position;
                    position.z = object->position.z - Player[0].position.z;
                    position.y = object->position.y - Player[0].position.y + Player[0].collideDir.y;
                    position.x = object->position.x - Player[0].position.x;

                    if (position.Magnitude() < 6.0)
                        object->type = OBJECT_RING_SPARKLE;
                    break;
                }

                case OBJECT_RING_SPARKLE: {
                    if (++object->timer > 15) {
                        object->timer = 0;
                        object->type  = OBJECT_NONE;
                    }
                    break;
                }

                case OBJECT_SPRING: {
                    Vector3D position;
                    position.z = object->position.z - Player[0].position.z;
                    position.y = object->position.y - Player[0].position.y + Player[0].collideDir.y;
                    position.x = object->position.x - Player[0].position.x;

                    if (position.Magnitude() < 6.0) {
                        Player[PNumber].velocity.y = 4.0;
                        Player[PNumber].gravity    = GRAVITY_AIR;
                    }
                    break;
                }
            }
        }
    }

    RingRotationY += 0.05f;
    if (RingRotationY > RSDK_PI * 2)
        RingRotationY -= RSDK_PI * 2;
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