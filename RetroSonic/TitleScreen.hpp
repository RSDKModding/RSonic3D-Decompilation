#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "RetroEngine.hpp"

extern InputData TitleInput;
extern TextMenu TitleMenu;
extern float data_4DA320;
extern float backpos[10];
extern float TYspeed;
extern float TXspeed;
extern float TailsY;
extern float TailsX;
extern int TAnimation;
extern int SAnimation;
extern float SYspeed;
extern float SXspeed;
extern float SonY;
extern float SonX;
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
extern float data_4DA3A8;

extern float data_41F59C;
extern int DebugEnabled;
extern int DebugMode;
extern byte DebugCode[12];

void ResetTitleScreen();
void ProcessTitleScreen();
void TitleScrMovement();
void DrawMenuBackground();
void DrawTitleScr(sbyte id);
void DrawGameMenu(TextMenu TextMenu, int x, int y);
void DrawScrollingMenu(TextMenu TextMenu, int x, int y, int clipT, int clipB, int scrollPos);
void LoadCharacterMenu(TextMenu *menu);
void SetMenuSelMode(TextMenu *menu, int id);
void SetCharacters(int row);
void CodeCheck();
void LoadZoneMenu(TextMenu *menu);
void Zone_TitleScreen_4127E6();
void DrawTitleModel(char type);

#endif // !TITLESCREEN_H