#include "RetroEngine.hpp"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!CreateMWindow(hInstance, nCmdShow)) {
        QuitMessage = EXIT_SUCCESS;
        PostQuitMessage(EXIT_SUCCESS);
    }

    InitObjectModels();
    InitZoneSurface();
    InitMTextSurface();

    GameMode = GAMEMODE_MAINGAME;

    if (QueryPerformanceFrequency(&frequency)) {
        dword_4DA2C0 = 1;
        FrameTicks   = frequency.QuadPart / 60;
        QueryPerformanceCounter(&FrameNextTicks);
        dbl_4DA278 = 1.0 / (double)frequency.QuadPart;
    }
    else {
        FrameNextTicks.QuadPart = timeGetTime();
        dbl_4DA278              = 0.001;
    }

    FrameCurrentTicks = FrameNextTicks.QuadPart;
    while (QuitMessage) {
        if (PeekMessageA(&msg, 0, 0, 0, 1)) {
            if (msg.message == 18)
                TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        else {
            if (dword_4DA2C0)
                QueryPerformanceCounter(&largeInt_4DA290);
            else
                largeInt_4DA290.QuadPart = timeGetTime();

            if (largeInt_4DA290.QuadPart > FrameNextTicks.QuadPart) {
                dbl_4DA280        = (double)(largeInt_4DA290.QuadPart - FrameCurrentTicks) * dbl_4DA278;
                FrameCurrentTicks = largeInt_4DA290.QuadPart;

                switch (GameMode) {
                    case GAMEMODE_TITLESCREEN: ProcessTitleScreen(); break;
                    case GAMEMODE_MAINGAME: ProcessMainGame(); break;
                    default: break;
                }

                FrameNextTicks.QuadPart = (uint32_t)FrameTicks + largeInt_4DA290.QuadPart;
            }
        }
    }

    ReleaseInputDevice();
    ReleaseGraphicsAPI();
    return msg.wParam;
}