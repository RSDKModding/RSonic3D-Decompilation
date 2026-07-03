#ifndef USERDATA_H
#define USERDATA_H

#if !RETRO_USE_ORIGINAL_CODE
extern char gamePath[0x100];
extern char modsPath[0x100];

struct SettingsData {
    bool skipStartMenu;
    bool skipStartMenu_Config;
    bool startFullScreen;
    bool borderless;
    bool vsync;
    int scalingMode;
    int windowScale;
    int screenWidth;
    int refreshRate;
};

extern SettingsData Settings;

void InitUserdata();
void WriteSettings();
#endif

#endif // !USERDATA_H