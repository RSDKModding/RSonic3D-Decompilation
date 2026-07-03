#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE
char gamePath[0x100];
char modsPath[0x100];

SettingsData Settings;

void InitUserdata()
{
    // userdata files are loaded from this directory
    sprintf(gamePath, "%s", BASE_PATH);
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
        ini.SetInteger("Window", "ScalingMode", Settings.scalingMode = 1);
        ini.SetInteger("Window", "WindowScale", Settings.windowScale = 2);

        ini.SetInteger("Window", "ScreenWidth", Settings.screenWidth = DEFAULT_SCREEN_XSIZE);
        SCREEN_XSIZE = Settings.screenWidth;

        ini.SetInteger("Window", "RefreshRate", Settings.refreshRate = 60);

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

        if (!ini.GetInteger("Window", "ScalingMode", &Settings.scalingMode))
            Settings.scalingMode = 0;

        if (!ini.GetInteger("Window", "WindowScale", &Settings.windowScale))
            Settings.windowScale = 2;

        if (!ini.GetInteger("Window", "ScreenWidth", &Settings.screenWidth))
            Settings.screenWidth = DEFAULT_SCREEN_XSIZE;

        SCREEN_XSIZE = Settings.screenWidth;

        if (!ini.GetInteger("Window", "RefreshRate", &Settings.refreshRate))
            Settings.refreshRate = 60;
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

    ini.SetComment("Window", "SMComment", "Determines what scaling is used. 0 is nearest neighbour, 1 is linear.");
    ini.SetInteger("Window", "ScalingMode", Settings.scalingMode);

    ini.SetComment("Window", "WSComment", "How big the window will be");
    ini.SetInteger("Window", "WindowScale", Settings.windowScale);

    ini.SetComment("Window", "SWComment", "How wide the base screen will be in pixels");
    ini.SetInteger("Window", "ScreenWidth", Settings.screenWidth);

    ini.SetComment("Window", "RRComment", "Determines the target FPS");
    ini.SetInteger("Window", "RefreshRate", Settings.refreshRate);

    char buffer[0x100];
    sprintf(buffer, "%ssettings.ini", gamePath);

    ini.Write(buffer, false);
}
#endif