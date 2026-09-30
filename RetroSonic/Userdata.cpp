#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE
char modsPath[0x100];

SettingsData Settings;

void InitUserdata()
{
#if RETRO_USE_MOD_LOADER
    sprintf(modsPath, "%s", BASE_PATH);
#endif

    char buffer[0x100];
    sprintf(buffer, BASE_PATH "settings.ini");

    FileIO *file = fOpen(buffer, "rb");
    if (!file) {
        IniParser ini;

        ini.SetBool("Game", "SkipStartMenu", Settings.skipStartMenu = true);
        Settings.skipStartMenu_Config = Settings.skipStartMenu;

        ini.SetBool("Window", "FullScreen", Settings.startFullScreen = DEFAULT_FULLSCREEN);
        ini.SetBool("Window", "Borderless", Settings.borderless = false);
        ini.SetBool("Window", "VSync", Settings.vsync = true);
        ini.SetInteger("Window", "WindowScale", Settings.windowScale = 2);

        ini.SetInteger("Window", "ScreenWidth", Settings.screenWidth = DEFAULT_SCREEN_XSIZE);
        SCREEN_XSIZE = Settings.screenWidth;

        ini.SetInteger("Window", "RefreshRate", Settings.refreshRate = 60);

#if RETRO_USE_SDL2 || RETRO_USE_SDL3
        ini.SetInteger("Keyboard 1", "Up", KEYBOARD_SCANCODE_UP = SDL_SCANCODE_UP);
        ini.SetInteger("Keyboard 1", "Down", KEYBOARD_SCANCODE_DOWN = SDL_SCANCODE_DOWN);
        ini.SetInteger("Keyboard 1", "Left", KEYBOARD_SCANCODE_LEFT = SDL_SCANCODE_LEFT);
        ini.SetInteger("Keyboard 1", "Right", KEYBOARD_SCANCODE_RIGHT = SDL_SCANCODE_RIGHT);
        ini.SetInteger("Keyboard 1", "A", KEYBOARD_SCANCODE_A = SDL_SCANCODE_Z);
        ini.SetInteger("Keyboard 1", "B", KEYBOARD_SCANCODE_B = SDL_SCANCODE_X);
        ini.SetInteger("Keyboard 1", "Start", KEYBOARD_SCANCODE_RETURN = SDL_SCANCODE_RETURN);
        ini.SetInteger("Keyboard 1", "Control", KEYBOARD_SCANCODE_LCTRL = SDL_SCANCODE_LCTRL);
        ini.SetInteger("Keyboard 1", "Shift", KEYBOARD_SCANCODE_LSHIFT = SDL_SCANCODE_LSHIFT);
#elif RETRO_USE_SDL1
        ini.SetInteger("Keyboard 1", "Up", KEYBOARD_SCANCODE_UP = SDLK_UP);
        ini.SetInteger("Keyboard 1", "Down", KEYBOARD_SCANCODE_DOWN = SDLK_DOWN);
        ini.SetInteger("Keyboard 1", "Left", KEYBOARD_SCANCODE_LEFT = SDLK_LEFT);
        ini.SetInteger("Keyboard 1", "Right", KEYBOARD_SCANCODE_RIGHT = SDLK_RIGHT);
        ini.SetInteger("Keyboard 1", "A", KEYBOARD_SCANCODE_A = SDLK_z);
        ini.SetInteger("Keyboard 1", "B", KEYBOARD_SCANCODE_B = SDLK_x);
        ini.SetInteger("Keyboard 1", "Start", KEYBOARD_SCANCODE_RETURN = SDLK_RETURN);
        ini.SetInteger("Keyboard 1", "Control", KEYBOARD_SCANCODE_LCTRL = SDLK_LCTRL);
        ini.SetInteger("Keyboard 1", "Shift", KEYBOARD_SCANCODE_LSHIFT = SDLK_LSHIFT);
#endif

        ini.Write(buffer);
    }
    else {
        fClose(file);
        IniParser ini(buffer, false);

        if (!ini.GetBool("Game", "SkipStartMenu", &Settings.skipStartMenu))
            Settings.skipStartMenu = false;
        Settings.skipStartMenu_Config = Settings.skipStartMenu;

        if (!ini.GetBool("Window", "FullScreen", &Settings.startFullScreen))
            Settings.startFullScreen = DEFAULT_FULLSCREEN;

        if (!ini.GetBool("Window", "Borderless", &Settings.borderless))
            Settings.borderless = false;

        if (!ini.GetBool("Window", "VSync", &Settings.vsync))
            Settings.vsync = false;

        if (!ini.GetInteger("Window", "WindowScale", &Settings.windowScale))
            Settings.windowScale = 2;

        if (!ini.GetInteger("Window", "ScreenWidth", &Settings.screenWidth))
            Settings.screenWidth = DEFAULT_SCREEN_XSIZE;

        SCREEN_XSIZE = Settings.screenWidth;

        if (!ini.GetInteger("Window", "RefreshRate", &Settings.refreshRate))
            Settings.refreshRate = 60;

#if RETRO_USE_SDL2 || RETRO_USE_SDL3
        if (!ini.GetInteger("Keyboard 1", "Up", &KEYBOARD_SCANCODE_UP))
            KEYBOARD_SCANCODE_UP = SDL_SCANCODE_UP;
        if (!ini.GetInteger("Keyboard 1", "Down", &KEYBOARD_SCANCODE_DOWN))
            KEYBOARD_SCANCODE_DOWN = SDL_SCANCODE_DOWN;
        if (!ini.GetInteger("Keyboard 1", "Left", &KEYBOARD_SCANCODE_LEFT))
            KEYBOARD_SCANCODE_LEFT = SDL_SCANCODE_LEFT;
        if (!ini.GetInteger("Keyboard 1", "Right", &KEYBOARD_SCANCODE_RIGHT))
            KEYBOARD_SCANCODE_RIGHT = SDL_SCANCODE_RIGHT;
        if (!ini.GetInteger("Keyboard 1", "A", &KEYBOARD_SCANCODE_A))
            KEYBOARD_SCANCODE_A = SDL_SCANCODE_Z;
        if (!ini.GetInteger("Keyboard 1", "B", &KEYBOARD_SCANCODE_B))
            KEYBOARD_SCANCODE_B = SDL_SCANCODE_X;
        if (!ini.GetInteger("Keyboard 1", "Start", &KEYBOARD_SCANCODE_RETURN))
            KEYBOARD_SCANCODE_RETURN = SDL_SCANCODE_RETURN;
        if (!ini.GetInteger("Keyboard 1", "Control", &KEYBOARD_SCANCODE_LCTRL))
            KEYBOARD_SCANCODE_LCTRL = SDL_SCANCODE_LCTRL;
        if (!ini.GetInteger("Keyboard 1", "Shift", &KEYBOARD_SCANCODE_LSHIFT))
            KEYBOARD_SCANCODE_LSHIFT = SDL_SCANCODE_LSHIFT;
#elif RETRO_USING_SDL1
        if (!ini.GetInteger("Keyboard 1", "Up", &KEYBOARD_SCANCODE_UP))
            KEYBOARD_SCANCODE_UP = SDLK_UP;
        if (!ini.GetInteger("Keyboard 1", "Down", &KEYBOARD_SCANCODE_DOWN))
            KEYBOARD_SCANCODE_DOWN = SDLK_DOWN;
        if (!ini.GetInteger("Keyboard 1", "Left", &KEYBOARD_SCANCODE_LEFT))
            KEYBOARD_SCANCODE_LEFT = SDLK_LEFT;
        if (!ini.GetInteger("Keyboard 1", "Right", &KEYBOARD_SCANCODE_RIGHT))
            KEYBOARD_SCANCODE_RIGHT = SDLK_RIGHT;
        if (!ini.GetInteger("Keyboard 1", "A", &KEYBOARD_SCANCODE_A))
            KEYBOARD_SCANCODE_A = SDLK_z;
        if (!ini.GetInteger("Keyboard 1", "B", &KEYBOARD_SCANCODE_B))
            KEYBOARD_SCANCODE_B = SDLK_x;
        if (!ini.GetInteger("Keyboard 1", "Start", &KEYBOARD_SCANCODE_RETURN))
            KEYBOARD_SCANCODE_RETURN = SDLK_RETURN;
        if (!ini.GetInteger("Keyboard 1", "Control", &KEYBOARD_SCANCODE_LCTRL))
            KEYBOARD_SCANCODE_LCTRL = SDLK_LCTRL;
        if (!ini.GetInteger("Keyboard 1", "Shift", &KEYBOARD_SCANCODE_LSHIFT))
            KEYBOARD_SCANCODE_LSHIFT = SDLK_LSHIFT;
#endif
    }
}

void WriteSettings()
{
    IniParser ini;

    ini.SetComment("Game", "SSMenuComment", "If set to true, disables the start menu");
    ini.SetBool("Game", "SkipStartMenu", Settings.skipStartMenu_Config);

    ini.SetComment("Window", "FSComment", "Determines if the window will be fullscreen or not");
    ini.SetBool("Window", "FullScreen", Settings.startFullScreen);

    ini.SetComment("Window", "BLComment", "Determines if the window will be borderless or not");
    ini.SetBool("Window", "Borderless", Settings.borderless);

    ini.SetComment("Window", "VSComment", "Determines if VSync will be active or not");
    ini.SetBool("Window", "VSync", Settings.vsync);

    ini.SetComment("Window", "WSComment", "How big the window will be");
    ini.SetInteger("Window", "WindowScale", Settings.windowScale);

    ini.SetComment("Window", "SWComment", "How wide the base screen will be in pixels");
    ini.SetInteger("Window", "ScreenWidth", Settings.screenWidth);

    ini.SetComment("Window", "RRComment", "Determines the target FPS");
    ini.SetInteger("Window", "RefreshRate", Settings.refreshRate);

#if RETRO_USE_SDL3
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL3/SDL_Scancode)");
#elif RETRO_USE_SDL2
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL2/SDL_Scancode)");
#elif RETRO_USE_SDL1
    ini.SetComment("Keyboard 1", "IK1Comment",
                   "Keyboard Mappings for P1 (Based on: https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html)");
#endif
    ini.SetInteger("Keyboard 1", "Up", KEYBOARD_SCANCODE_UP);
    ini.SetInteger("Keyboard 1", "Down", KEYBOARD_SCANCODE_DOWN);
    ini.SetInteger("Keyboard 1", "Left", KEYBOARD_SCANCODE_LEFT);
    ini.SetInteger("Keyboard 1", "Right", KEYBOARD_SCANCODE_RIGHT);
    ini.SetInteger("Keyboard 1", "A", KEYBOARD_SCANCODE_A);
    ini.SetInteger("Keyboard 1", "B", KEYBOARD_SCANCODE_B);
    ini.SetInteger("Keyboard 1", "Start", KEYBOARD_SCANCODE_RETURN);
    ini.SetInteger("Keyboard 1", "Control", KEYBOARD_SCANCODE_LCTRL);
    ini.SetInteger("Keyboard 1", "Shift", KEYBOARD_SCANCODE_LSHIFT);

    ini.Write(BASE_PATH "settings.ini");
}
#endif