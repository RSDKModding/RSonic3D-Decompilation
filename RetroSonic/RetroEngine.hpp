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

#include "coldet/src/cdmath3d.h"
#include "coldet/src/coldet.h"
#include "FreeImage/Dist/x32/FreeImage.h"

#define SCREEN_XSIZE (320)
#define SCREEN_YSIZE (240)

#define SCREEN_XSIZE_F (320.0f)
#define SCREEN_YSIZE_F (240.0f)

#define SCREEN_XCENTER (SCREEN_XSIZE / 2)
#define SCREEN_YCENTER (SCREEN_YSIZE / 2)

#define SCREEN_XCENTER_F (SCREEN_XSIZE_F / 2.0f)
#define SCREEN_YCENTER_F (SCREEN_YSIZE_F / 2.0f)

// ============================
// ENGINE INCLUDES
// ============================

#include "DX7/DX7Include.h"

#include "Player.hpp"
#include "Collision.hpp"
#include "Drawing.hpp"
#include "Reader.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "String.hpp"
#include "Text.hpp"

#include "Misc.h"

#include "MainGame.hpp"
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
extern float CameraCullX;
extern float CameraCullZ;

extern Vector3D CameraPosition;
extern int32_t SupportsZBufferFmt;
extern uint8_t D3DDeviceType;
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