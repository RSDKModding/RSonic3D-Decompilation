#ifndef RETROSONICENGINE_H
#define RETROSONICENGINE_H

// ============================
// STANDARD LIBS
// ============================

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cstdint>
#include <stdio.h>
#include <new>
#include <math.h>
#include <mmsystem.h>

// ============================
// STANDARD TYPES
// ============================

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;

#include "resource.h"

#define D3D_OVERLOADS
#include "d3d.h"
#include "dinput.h"

#include "coldet/src/cdmath3d.h"
#include "coldet/src/coldet.h"
#include "FreeImage/Dist/x32/FreeImage.h"

// ============================
// ENGINE INCLUDES
// ============================

#include "Player.hpp"
#include "Collision.hpp"
#include "Drawing.hpp"
#include "Reader.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Drawing.hpp"
#include "Stage.h"
#include "Text.hpp"

#include "Misc.h"

#include "ZoneTest.h"
#include "TitleScreen.hpp"

extern double dbl_4DA278;
extern double dbl_4DA280;

extern MSG msg;

extern LARGE_INTEGER frequency;
extern LARGE_INTEGER largeInt_4DA290;
extern LONGLONG FrameCurrentTicks;
extern HINSTANCE hInstance;

extern uint8_t GameMode;
extern int32_t FrameTicks;
extern int32_t dword_4DA2C0;
extern LARGE_INTEGER FrameNextTicks;

extern float data_4C9D4C;
extern float data_4C9D50;
extern float data_4C9D58;
extern float data_4C9D5C;
extern float data_4C9D60;
extern float data_4C9D64;
extern float data_4C9D68;
extern float data_4C9D6C;
extern float data_4C9D70;
extern Vector3D vector_4C9D74;
extern int32_t data_4C9D80;
extern uint8_t data_4C9D84;
extern int32_t QuitMessage;
extern BOOL HasGameStarted;

extern HMENU hMenu;
extern char *StrWindowClassErr;

enum WindowScalingMode {
    SCALE_1X,
    SCALE_2X,
    SCALE_3X,
    SCALE_4X,
};

enum GameModeType {
    GAMEMODE_TITLESCREEN,
    GAMEMODE_MAINGAME,
};

BOOL CreateMWindow(_In_ HINSTANCE hInstance, _In_ int nCmdShow);
LRESULT CALLBACK MWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void SetHasGameStarted(BOOL hasStarted);
void SetShouldSkipTitle(uint8_t shouldSkip);

#endif // !RETROSONICENGINE_H