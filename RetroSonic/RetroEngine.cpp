#include "RetroEngine.hpp"

double dbl_4DA278;
double dbl_4DA280;

MSG msg;

LARGE_INTEGER frequency;
LARGE_INTEGER largeInt_4DA290;
LONGLONG FrameCurrentTicks;
HINSTANCE hInstance;

uint8_t GameMode;
int32_t FrameTicks = 16;
int32_t dword_4DA2C0;
LARGE_INTEGER FrameNextTicks;

float data_4C9D4C;
float data_4C9D50;
float data_4C9D58;
float data_4C9D5C;
float data_4C9D60;
float data_4C9D64;
float data_4C9D68;
float data_4C9D6C;
float data_4C9D70;
Vector3D vector_4C9D74;
int32_t data_4C9D80;
uint8_t data_4C9D84;
int32_t QuitMessage = EXIT_FAILURE;
BOOL HasGameStarted;

HMENU hMenu;

const char *StrResolutionOptions[4] = { "320x240", "640x480", "800x600", "1024x768" };
char *StrWindowClassErr;

BOOL CreateMWindow(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
    WNDCLASSA wCA;
    wCA.style         = CS_VREDRAW | CS_HREDRAW;
    wCA.lpfnWndProc   = MWindowProc;
    wCA.cbClsExtra    = 0;
    wCA.cbWndExtra    = 4;
    wCA.hInstance     = hInstance;
    wCA.hIcon         = LoadIconA(hInstance, MAKEINTRESOURCEA(0x65));
    wCA.hCursor       = LoadCursorA(0, MAKEINTRESOURCEA(0x7F00));
    wCA.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wCA.lpszMenuName  = (LPCSTR)IDR_MAINMENU;
    wCA.lpszClassName = "Retro-Sonic 3D: A Taxman Test";

    if (RegisterClassA(&wCA)) {
        int cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);
        int cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME);
        int cyCaption   = GetSystemMetrics(SM_CYCAPTION);
        int cyMenu      = GetSystemMetrics(SM_CYMENU);

        int w = R3D_SCALE(2, cxSizeFrame) + R3D_SCALE(320, 2);
        int h = R3D_SCALE(2, cySizeFrame) + R3D_SCALE(240, 2);

        int dwStyle = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

        HWND hWnd = CreateWindowA("Retro-Sonic 3D: A Taxman Test", "Retro-Sonic 3D: A Taxman Test", dwStyle, 0x80000000, 0x80000000, w,
                                  (cyMenu + cyCaption) + h, 0, 0, hInstance, 0);

        if (hWnd) {
            ShowWindow(hWnd, nCmdShow);
            UpdateWindow(hWnd);
            SetHasGameStarted(TRUE);
            if (InitInputDevice(hWnd, hInstance))
                return TryInitDirectDraw(hWnd);
        }
    }
    else {
        StrWindowClassErr = (char *)("Error Registering Window Class");
    }

    return FALSE;
}

void SetScreenResolution(sbyte resolution, sbyte windowResolution, sbyte colourDepth)
{
    switch (resolution) {
        case SCALE_1X:
            ResX = 320;
            ResY = 240;
            break;

        case SCALE_2X:
            ResX = 640;
            ResY = 480;
            break;

        case SCALE_3X:
            ResX = 800;
            ResY = 600;
            break;

        case SCALE_4X:
            ResX = 1024;
            ResY = 768;
            break;

        default: break;
    }

    switch (windowResolution) {
        case SCALE_1X:
            ResX = 320;
            ResY = 240;
            break;

        case SCALE_2X:
            ResX = 640;
            ResY = 480;
            break;

        default: break;
    }

    ColourDepth = colourDepth;
}

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
                        ReleaseSurfaceID(i);
                    }

                    InitMTextSurface();
                    break;

                case VK_F4:
                    ResetWindow(hWnd);
                    if (GameMode == GAMEMODE_TITLESCREEN)
                        InitMTextSurface();
                    break;

                case VK_F5: DialogBoxParamA(hInstance, "DMSelect", hWnd, MDialogProc, 0); break;
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

                case IDM_SET_DISPLAY: DialogBoxParamA(hInstance, "DMSelect", hWnd, MDialogProc, 0); break;

                case IDM_OPT_FULLSCREEN:
                    ResetWindow(hWnd);
                    if (GameMode == GAMEMODE_TITLESCREEN)
                        InitMTextSurface();
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
                SendDlgItemMessageA(hWnd, IDC_DM_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)StrResolutionOptions[i]);
            }

            for (int i = 0; i < 2; ++i) {
                SendDlgItemMessageA(hWnd, IDC_DM_WINDOW, CB_ADDSTRING, 0, (LPARAM)StrResolutionOptions[i]);
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

void SetHasGameStarted(BOOL hasStarted) { HasGameStarted = hasStarted; }

void SetShouldSkipTitle(uint8_t shouldSkip) { GameMode = shouldSkip; }