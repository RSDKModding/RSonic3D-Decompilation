#pragma once
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
extern uint8_t CodePos;
extern int SMenuY1;
extern int SMenuY2;
extern int MusicNo;
extern int SStageNo;
extern float data_4DA3A8;

extern float_t data_41F59C;
extern int32_t DebugEnabled;
extern int32_t DebugMode;
extern uint8_t DebugCode[12];

void ProcessTitleScreen();
void CodeCheck();
void SetMenuSelMode(TextMenu *menu, int32_t p_1);
TextMenu LoadCharacterMenu();
TextMenu LoadZoneMenu();
void DrawGameMenu(TextMenu p_0, int32_t p_1, uint8_t p_2);
void DrawScrollingMenu(TextMenu p_0, int32_t p_1, int32_t p_2, int32_t p_3, int32_t p_4, int32_t p_5);
void DrawTitleScr(char a1);
void TitleScrMovement();
void Zone_TitleScreen_4127E6();
void Zone_TitleScreen_4128A3();
void Zone_TitleScreen_4128CF();
void DrawMenuBackground();
void Zone_TitleScreen_4129E4();
void ResetTitleScreen();