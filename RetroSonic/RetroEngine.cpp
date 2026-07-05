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
    wndClass.lpfnWndProc   = WindowProc;
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

            GameRunning = true;
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
    if (WindowMode == false)
        flags |= SDL_WINDOW_FULLSCREEN;
#elif RETRO_USE_SDL2
    uint flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (WindowMode == false)
        flags |= SDL_WINDOW_FULLSCREEN;
#elif RETRO_USE_SDL1
    uint flags = SDL_OPENGL;
    if (WindowMode == false)
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

    GameRunning = true;
    if (InitInputDevice() && InitGraphicsAPI())
        return true;

    return false;
#endif
}

#if RETRO_USE_ORIGINAL_CODE
LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

                case VK_F5: DialogBoxParam(HInst, "DMSelect", hWnd, DialogProc, 0); break;
            }

            return DefWindowProc(hWnd, Msg, wParam, lParam);

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

                case IDM_SET_DISPLAY: DialogBoxParam(HInst, "DMSelect", hWnd, DialogProc, 0); break;

                case IDM_OPT_FULLSCREEN:
                    ResetWindow(hWnd);
                    if (GameMode == GAMEMODE_TITLESCREEN)
                        LoadFontAssets();
                    break;

                case IDM_ERASE_SAVE: break;
            }

            return 0;

        default: return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
        case WM_INITDIALOG:
            for (int i = 0; i < 4; ++i) {
                SendDlgItemMessage(hWnd, IDC_DM_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)ResolutionOptions[i]);
            }

            for (int i = 0; i < 2; ++i) {
                SendDlgItemMessage(hWnd, IDC_DM_WINDOW, CB_ADDSTRING, 0, (LPARAM)ResolutionOptions[i]);
            }

            SendDlgItemMessage(hWnd, IDC_DM_RESOLUTION, CB_SETCURSEL, 0, 0);
            SendDlgItemMessage(hWnd, IDC_DM_WINDOW, CB_SETCURSEL, 0, 0);
            SendDlgItemMessage(hWnd, IDC_DM_STRETCH, BM_SETCHECK, BST_CHECKED, 0);
            return 1;

        case WM_COMMAND:
            if (wParam == IDC_DM_OK) {
                char fullscreen = SendDlgItemMessage(hWnd, IDC_DM_RESOLUTION, CB_GETCURSEL, 0, 0);
                char window     = SendDlgItemMessage(hWnd, IDC_DM_RESOLUTION, CB_GETCURSEL, 0, 0);

                char centerScreen = SendDlgItemMessage(hWnd, IDC_DM_CENTER, BM_GETCHECK, BST_CHECKED, 0);
                char stretchFit   = SendDlgItemMessage(hWnd, IDC_DM_STRETCH, BM_GETCHECK, BST_CHECKED, 0);
                char sai2X        = SendDlgItemMessage(hWnd, IDC_DM_2XSAI, BM_GETCHECK, BST_CHECKED, 0);

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
                GameRunning = false;
                break;

#if RETRO_USE_SDL3
            case SDL_EVENT_WINDOW_FOCUS_GAINED: EnableInput(); break;
            case SDL_EVENT_WINDOW_FOCUS_LOST: DisableInput(); break;

            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                glViewport(0, 0, SDLEvent.window.data1, SDLEvent.window.data2);
                break;
            }
#elif RETRO_USE_SDL2
            case SDL_WINDOWEVENT:
                switch (SDLEvent.window.event) {
                    case SDL_WINDOWEVENT_FOCUS_GAINED: EnableInput(); break;
                    case SDL_WINDOWEVENT_FOCUS_LOST: DisableInput(); break;

                    case SDL_WINDOWEVENT_SIZE_CHANGED: {
                        int w = 0;
                        int h = 0;
                        SDL_GL_GetDrawableSize(Window, &w, &h);
                        glViewport(0, 0, w, h);
                        break;
                    }

                    default: break;
                }
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

                    case SDLK_ESCAPE: GameRunning = false; break;

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
            CameraPosition.y = 10.0f;
            CameraPosition.z = -32.0f;

            MainGameMode = 2;

            // x-axis line of 6 rings
            for (ObjectLoop = 0; ObjectLoop < 6; ++ObjectLoop) {
                Object *ring = &LevelObjects[ObjectLoop];

                ring->type    = OBJ_TYPE_RING;
                ring->enabled = true;

                ring->position.x = -(ObjectLoop * 8.0f) - 10.0f;
                ring->position.z = -20.0f;
                ring->position.y = 5.0f;
            }

            // z-axis line of 4 rings
            for (ObjectLoop = 10; ObjectLoop < 14; ++ObjectLoop) {
                Object *ring = &LevelObjects[ObjectLoop];

                ring->type       = OBJ_TYPE_RING;
                ring->enabled    = true;
                ring->position.z = (ObjectLoop * 8.0f) - 60.0f;
                ring->position.x = -50.0f;
                ring->position.y = 12.0f;
            }

            // 1-element for loop that spawns the spring object, lol
            for (ObjectLoop = 14; ObjectLoop < 15; ++ObjectLoop) {
                Object *spring = &LevelObjects[ObjectLoop];

                spring->type       = OBJ_TYPE_SPRING;
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

                    ProcessPlayerAnimation();
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

void SetGameMode(int mode) { GameMode = mode; }

void PauseCheck()
{
    if (MGameInput.start == true) {
        if (PauseV == true)
            PauseV = false;
        else if (PauseV == false)
            PauseV = true;
    }
}

void SetSceneRenderProperties(byte id)
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