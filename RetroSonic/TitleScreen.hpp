#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "RetroEngine.hpp"

enum TitleScreenModes {
    TITLE_TAXMAN_PRESENTS,
    TITLE_LOGO_SCALE,
    TITLE_MAIN,
    TITLE_CHARACTER_MENU,
    TITLE_ZONE_MENU,
    TITLE_SAVE_MENU,
    TITLE_DEBUG_MENU,
};

enum TitleOptions {
    OPTION_NEW_GAME,
    OPTION_CONTINUE_GAME,
    OPTION_DEBUG_OPTIONS,
    OPTION_EXIT_GAME,
};

extern InputData TitleInput;
extern TextMenu TitleMenu;

extern float BackXRotation;
extern float backpos[10];

extern float SonX;
extern float SonY;
extern float SonZ;
extern float SXspeed;
extern float SYspeed;
extern int SAnimation;
extern float SAnimationSpeed;

extern float TailsX;
extern float TailsY;
extern float TailsZ;
extern float TXspeed;
extern float TYspeed;
extern int TAnimation;
extern float TAnimationSpeed;

extern int TitleScreenMode;
extern int TtlTime;

extern char TxtStyle;

extern float RysS;
extern float SwS;

extern int AllStages;
extern byte CodePos;

extern int SMenuY1;
extern int SMenuY2;

extern int MusicNo;
extern int SStageNo;

extern int DebugEnabled;
extern int DebugMode;
extern byte DebugCode[12];

void ResetTitleScreen();
void ProcessTitleScreen();
void TitleScrMovement();
void SetMenuSelMode(TextMenu *menu, int id);
void SetCharacters(int id);
void CodeCheck();
void DrawMenuBackground();
void DrawTitleScr();
void DrawGameMenu(TextMenu Menu, int x, int y);
void DrawScrollingMenu(TextMenu Menu, int x, int y, int clipT, int clipB, int scrollPos);
void DrawTitleModel(byte type);
void LoadCharacterMenu(TextMenu *menu);
void LoadZoneMenu(TextMenu *menu);

#endif // !TITLESCREEN_H