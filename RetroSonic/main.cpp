#include "RetroEngine.hpp"

#if defined(_WIN32)
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    UNUSED(hPrevInstance);
    UNUSED(lpCmdLine);

#if RETRO_USE_ORIGINAL_CODE
    HInst    = hInstance;
    NCmdShow = nCmdShow;
#else
    UNUSED(hInstance);
    UNUSED(nCmdShow);
#endif
#else
int main(int argc, char **argv)
{
#endif
#if !RETRO_USE_ORIGINAL_CODE
#if RETRO_USE_SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
#elif RETRO_USE_SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
#endif
        char buffer[0x40];
        snprintf(buffer, sizeof(buffer), "Failed to initialize SDL: %s", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Initialization Error", buffer, NULL);
        return EXIT_FAILURE;
    }
#endif

    if (!CreateMWindow()) {
        QuitMessage = EXIT_SUCCESS;
#if RETRO_USE_ORIGINAL_CODE
        PostQuitMessage(QuitMessage);
#endif
    }

    InitObjectModels();
    InitZoneSurface();
    InitMTextSurface();

    GameMode = GAMEMODE_MAINGAME;

#if RETRO_USE_ORIGINAL_CODE
    if (QueryPerformanceFrequency((LARGE_INTEGER *)(&Frequency))) {
        UseQueryCounter = true;

        FrameTicks = Frequency / 60;
        QueryPerformanceCounter((LARGE_INTEGER *)(&FrameNextTicks));
        FrameSecondsPerTick = 1.0 / Frequency;
    }
    else {
        UseQueryCounter = false;

        FrameNextTicks      = timeGetTime();
        FrameSecondsPerTick = 0.001;
    }
#else
    Frequency = SDL_GetPerformanceFrequency();

    FrameTicks     = Frequency / 60;
    FrameNextTicks = SDL_GetPerformanceCounter();

    FrameSecondsPerTick = 1.0 / Frequency;

    SDL_Event event = {};
#endif

    FrameLastTicks = FrameNextTicks;

#if RETRO_USE_ORIGINAL_CODE
    while (QuitMessage) {
#else
    while (EngineRunning != false) {
#endif

#if RETRO_USE_ORIGINAL_CODE
        if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
            if (Message.message == WM_QUIT)
                TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
        else {
            if (UseQueryCounter)
                QueryPerformanceCounter((LARGE_INTEGER *)(&FrameCurrentTicks));
            else
                FrameCurrentTicks = timeGetTime();
#else
            ProcessEvents(event);
            FrameCurrentTicks = SDL_GetPerformanceCounter();
#endif

            if (FrameCurrentTicks > FrameNextTicks) {
                FrameDeltaTime = (FrameCurrentTicks - FrameLastTicks) * FrameSecondsPerTick;
                FrameLastTicks = FrameCurrentTicks;

                switch (GameMode) {
                    case GAMEMODE_TITLESCREEN: ProcessTitleScreen(); break;
                    case GAMEMODE_MAINGAME: ProcessMainGame(); break;
                    default: break;
                }

                FrameNextTicks = FrameTicks + FrameCurrentTicks;
            }

#if RETRO_USE_ORIGINAL_CODE
        }
#endif
    }

    ReleaseInputDevice();
    ReleaseGraphicsAPI();

#if RETRO_USE_ORIGINAL_CODE
    return Message.wParam;
#else
    return QuitMessage;
#endif
}