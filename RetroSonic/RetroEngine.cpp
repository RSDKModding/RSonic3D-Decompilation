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
#else
SDL_Event SDLEvent;
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
    wndClass.hIcon         = LoadIcon(HInst, MAKEINTRESOURCE(IDI_MAINICON));
    wndClass.hCursor       = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
    wndClass.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
    wndClass.lpszMenuName  = (LPCSTR)(IDR_MAINMENU);
    wndClass.lpszClassName = title;

    if (RegisterClassA(&wndClass)) {
        int width  = 2 * GetSystemMetrics(SM_CXSIZEFRAME);
        int height = 2 * GetSystemMetrics(SM_CYSIZEFRAME);

        width += 2 * SCREEN_XSIZE;
        height += 2 * SCREEN_YSIZE;

        height += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU);

        int style = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

        HWnd = CreateWindow(title, title, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, HInst, 0);
        if (HWnd != NULL) {
            ShowWindow(HWnd, NCmdShow);
            UpdateWindow(HWnd);

            EngineRunning = true;
            if (InitInputDevice() && InitGraphicsAPI())
                return true;

            return false;
        }
    }
    else {
        MessageBox(HWnd, "Error Registering Window Class", 0, MB_ICONHAND);
    }

    return false;
#else
    SetScreenResolution(0, 0, 0);

#if RETRO_USE_SDL3
    uint flags = SDL_WINDOW_OPENGL;
    if (WindowMode == 0)
        flags |= SDL_WINDOW_FULLSCREEN;
#elif RETRO_USE_SDL2
    uint flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (WindowMode == 0)
        flags |= SDL_WINDOW_FULLSCREEN;
#elif RETRO_USE_SDL1
    uint flags = SDL_OPENGL;
    if (WindowMode == 0)
        flags |= SDL_FULLSCREEN;
#endif

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

#if RETRO_USE_SDL3
    Window = SDL_CreateWindow("Retro-Sonic 3D: A Taxman Test", ResX, ResY, flags);
#elif RETRO_USE_SDL2
    Window = SDL_CreateWindow("Retro-Sonic 3D: A Taxman Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ResX, ResY, flags);
#elif RETRO_USE_SDL1
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, Settings.vsync);
    SDL_WM_SetCaption("Retro-Sonic 3D: A Taxman Test", NULL);
    Window = SDL_SetVideoMode(ResX, ResY, 32, flags);
#endif
    if (!Window)
        return false;

#if RETRO_USE_SDL3 || RETRO_USE_SDL2
    GLContext = SDL_GL_CreateContext(Window);
    if (!GLContext)
        return false;

    SDL_GL_SetSwapInterval(Settings.vsync);

    if (Settings.borderless)
        SDL_SetWindowBordered(Window, false);
#endif

    EngineRunning = true;
    if (InitInputDevice() && InitGraphicsAPI())
        return true;

    return false;
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

                    for (int i = 0; i < 7; ++i) {
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

                case IDM_HELP_ABOUT:
                    // This'd be the about menu, nothing here though :(
                    break;

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
    if (WindowMode == true) {
        GetClientRect(hWnd, &ClientRect);
        GetClientRect(hWnd, &WindowRect);
        ClientToScreen(hWnd, (POINT *)&WindowRect.left);
        ClientToScreen(hWnd, (POINT *)&WindowRect.right);
    }
}

void ResetWindow(HWND hWnd)
{
    if (WindowMode == true)
        GetWindowRect(hWnd, &WindowStoreRect);

    ToggleScreenMode();
}
#else
void ProcessEvents()
{
    while (SDL_PollEvent(&SDLEvent)) {
        switch (SDLEvent.type) {
#if RETRO_USE_SDL3
            case SDL_EVENT_QUIT:
#elif RETRO_USE_SDL2 || RETRO_USE_SDL1
            case SDL_QUIT:
#endif
                EngineRunning = false;
                break;

#if RETRO_USE_SDL3
            case SDL_EVENT_WINDOW_FOCUS_GAINED: EnableInput(); break;
            case SDL_EVENT_WINDOW_FOCUS_LOST: DisableInput(); break;

#elif RETRO_USE_SDL2
            case SDL_WINDOWEVENT:
                if (SDLEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                    EnableInput();

                if (SDLEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                    DisableInput();
                break;

#elif RETRO_USE_SDL1
            case SDL_ACTIVEEVENT:
                if (SDLEvent.active.state & SDL_APPINPUTFOCUS) {
                    if (SDLEvent.active.gain)
                        EnableInput();
                    else
                        DisableInput();
                }
                break;
#endif

#if RETRO_USE_SDL3
            case SDL_EVENT_KEY_DOWN:
                switch (SDLEvent.key.key) {
#elif RETRO_USE_SDL2 || RETRO_USE_SDL1
            case SDL_KEYDOWN:
                switch (SDLEvent.key.keysym.sym) {
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
            CameraTargetPosition.y = 10.0f;
            CameraTargetPosition.z = -32.0f;
            MainGameMode           = 2;

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

        case 1: break;

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

                PNumber = 0;
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
        if (PauseV == true)
            PauseV = false;
        else if (PauseV == false)
            PauseV == true;
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

    if (!Player[PNumber].disableControl) {
        Player[PNumber].targetAngle = 0;

        CheckInput(&MGameInput);

        if (Debug) {
            if (MGameInput.left == 1) {
                Player[0].position.x   = Player[0].position.x - Cos(CameraRotateY);
                CameraTargetPosition.x = CameraTargetPosition.x - Cos(CameraRotateY);
                Player[0].position.z   = Player[0].position.z - Sin(CameraRotateY);
                CameraTargetPosition.z = CameraTargetPosition.z - Sin(CameraRotateY);
            }
            if (MGameInput.right == 1) {
                Player[0].position.x   = Cos(CameraRotateY) + Player[0].position.x;
                CameraTargetPosition.x = Cos(CameraRotateY) + CameraTargetPosition.x;
                Player[0].position.z   = Sin(CameraRotateY) + Player[0].position.z;
                CameraTargetPosition.z = Sin(CameraRotateY) + CameraTargetPosition.z;
            }
            if (MGameInput.Z) {
                if (MGameInput.up == 1) {
                    Player[0].position.y += 1.0;
                    CameraTargetPosition.y += 1.2;
                }
                if (MGameInput.down == 1) {
                    Player[0].position.y -= 1.0f;
                    CameraTargetPosition.y -= 1.2f;
                }
            }
            else {
                if (MGameInput.up == 1) {
                    v2                     = CameraRotateY - 3.1415927 * 0.5;
                    Player[0].position.x   = Player[0].position.x - Cos(v2);
                    v3                     = CameraRotateY - 3.1415927 * 0.5;
                    CameraTargetPosition.x = CameraTargetPosition.x - Cos(v3);
                    v4                     = CameraRotateY - 3.1415927 * 0.5;
                    Player[0].position.z   = Player[0].position.z - Sin(v4);
                    v5                     = CameraRotateY - 3.1415927 * 0.5;
                    CameraTargetPosition.z = CameraTargetPosition.z - Sin(v5);
                }
                if (MGameInput.down == 1) {
                    v6                     = CameraRotateY - 3.1415927 * 0.5;
                    Player[0].position.x   = Cos(v6) + Player[0].position.x;
                    v7                     = CameraRotateY - 3.1415927 * 0.5;
                    CameraTargetPosition.x = Cos(v7) + CameraTargetPosition.x;
                    v8                     = CameraRotateY - 3.1415927 * 0.5;
                    Player[0].position.z   = Sin(v8) + Player[0].position.z;
                    v9                     = CameraRotateY - 3.1415927 * 0.5;
                    CameraTargetPosition.z = Sin(v9) + CameraTargetPosition.z;
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
                Player[PNumber].targetAngle += 64;
                if (Player[PNumber].angle > 192)
                    Player[PNumber].angle -= 256;
                ++v18;
            }

            if (MGameInput.right == 1) {
                if (Player[PNumber].angle < 64)
                    Player[PNumber].angle += 256;
                Player[PNumber].targetAngle += 192;
                ++v18;
            }

            if (MGameInput.up == 1) {
                if (Player[PNumber].angle > 128)
                    Player[PNumber].targetAngle += 256;
                ++v18;
            }

            if (MGameInput.down == 1) {
                Player[PNumber].targetAngle += 128;
                ++v18;
            }

            if (v18 > 0) {
                Player[PNumber].up = true;
                Player[PNumber].targetAngle /= v18;
            }

            Player[PNumber].z = MGameInput.Z == true;
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

    if (player->state == STATE_AIR && player->up == true) {
        if (player->angle < player->targetAngle) {
            player->angle += 6;

            if (player->speed > 0.0f)
                player->speed -= 0.02f;

            if (player->angle > player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->angle > player->targetAngle) {
            player->angle -= 6;

            if (player->speed > 0.0f)
                player->speed -= 0.02;

            if (player->angle < player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->speed < 2.4f)
            player->speed += 0.012f;
    }
    else if (player->state == STATE_GROUND && player->up == true) {
        if (player->angle < player->targetAngle) {
            player->angle += 8;

            if (player->speed > 0.0f)
                player->speed -= 0.01f;

            if (player->angle > player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->angle > player->targetAngle) {
            player->angle -= 8;

            if (player->speed > 0.0f)
                player->speed -= 0.01f;

            if (player->angle < player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->speed < 2.4f)
            player->speed += 0.012f;
    }
    else {
        if (player->speed > 0.0f)
            player->speed -= 0.012f;

        if (player->speed > -0.05f && player->speed < 0.0f)
            player->speed = 0.0f;
    }

    if (player->state != STATE_STATIC) {
        if (player->gravity == GRAVITY_AIR) {
            player->state = STATE_AIR;

            player->velocity.y -= 0.05f;
            if (player->velocity.y > 4.0f) {
                player->velocity.y = 4.0f;
                ResetPlayerRotation();
            }

            SetPlayerAnimationID(ANI_JUMPING, (player->speed * 0.4f) + 0.4f);
        }
        else {
            PlayerObject *player1 = &Player[0];
            if (player1->speed < 0.01f)
                SetPlayerAnimationID(ANI_STOPPED, 0.0f);
            else
                SetPlayerAnimationID(ANI_WALKING, player->speed * 0.4f);

            player->state = STATE_GROUND;
            if (player->speed == 0.0f)
                player->unused5 = 0;

            if (player->jumpPress == true) {
                player->gravity    = GRAVITY_AIR;
                player->velocity.y = 2.0f;
                player->state      = STATE_AIR;
                ResetPlayerRotation();
            }
        }
    }

    player->velocity.x = -Sin(player->rotationY) * player->speed;
    player->velocity.z = Cos(player->rotationY) * player->speed;

    player->collisionPos = { 0.0f, -4.0f, 0.0f };
    if (player->gravity == GRAVITY_AIR) {
        if (player->velocity.y < 0.0f) {
            if (ObjectFloorCollision(&player->position, { player->velocity.x, -player->velocity.y, player->velocity.z }) == true) {
                player->velocity.y = 0.0f;
                player->gravity    = GRAVITY_GROUND;
                player->state      = STATE_GROUND;
            }
        }
    }
    else {
        player->velocity.y = 0.0f;
        PlayerRotationPhysics(&player->velocity.x, &player->velocity.y, &player->velocity.z);

        player->collisionPos = { 0.0f, -3.5f, 0.0f };
        PlayerRotationPhysics(&player->collisionPos.x, &player->collisionPos.y, &player->collisionPos.z);

        player->position.x -= player->collisionPos.x * 0.75f;
        player->position.y -= player->collisionPos.y * 0.75f;
        player->position.z -= player->collisionPos.z * 0.75f;
        switch (ObjectFloorCollision(&player->position, player->collisionPos)) {
            case 1:
            case 2:
                player->gravity = GRAVITY_GROUND;
                player->state   = STATE_GROUND;
                break;

            case 0:
                player->position.x += player->collisionPos.x * 0.75f;
                player->position.y += player->collisionPos.y * 0.75f;
                player->position.z += player->collisionPos.z * 0.75f;
                player->gravity = GRAVITY_AIR;
                break;

            default: break;
        }
    }

    player->position += player->velocity;
    ProcessPlayerCamera();

    if (player->up == true)
        player->rotationY = (player->angle * RSDK_PI / 128.0f) + CameraRotateY;

    if (player->gravity == GRAVITY_AIR) {
        if (CameraAirTimer < 30)
            CameraAirTimer++;

        if (CameraRotateX < player->velocity.y) {
            CameraRotateX += 0.1f;
            if (CameraRotateX > player->velocity.y)
                CameraRotateX = player->velocity.y;
        }

        if (CameraRotateX > player->velocity.y) {
            CameraRotateX -= 0.1f;
            if (CameraRotateX < player->velocity.y)
                CameraRotateX = player->velocity.y;
        }
    }
    else {
        if (CameraAirTimer > 0)
            CameraAirTimer--;

        if (CameraRotateX > 0.0f) {
            CameraRotateX -= 0.1f;
            if (CameraRotateX < 0.0f)
                CameraRotateX = 0.0f;
        }

        if (CameraRotateX < 0.0f) {
            CameraRotateX += 0.1f;
            if (CameraRotateX > 0.0f)
                CameraRotateX = 0.0f;
        }
    }

    CameraTargetPosition.y = player->position.y + 15.0f - (CameraAirTimer * 0.25f * CameraRotateX);
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
                    position.y = object->position.y - Player[0].position.y + Player[0].collisionPos.y;
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
                    position.y = object->position.y - Player[0].position.y + Player[0].collisionPos.y;
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