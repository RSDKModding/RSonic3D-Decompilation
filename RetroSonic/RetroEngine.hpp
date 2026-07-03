#ifndef RETROSONICENGINE_H
#define RETROSONICENGINE_H

// ============================
// STANDARD LIBS
// ============================

#if RETRO_USE_ORIGINAL_CODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <cstring>
#include <new>
#include <math.h>
#if RETRO_USE_ORIGINAL_CODE
#include <mmsystem.h>
#endif

// ============================
// STANDARD TYPES
// ============================

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;

// ============================
// ENGINE DEFINES
// ============================

#define DEFAULT_SCREEN_XSIZE (320)
#define DEFAULT_SCREEN_YSIZE (240)

#if RETRO_USE_ORIGINAL_CODE
#define SCREEN_XSIZE (DEFAULT_SCREEN_XSIZE)
#endif

#define SCREEN_YSIZE (DEFAULT_SCREEN_YSIZE)

#define SCREEN_XSIZE_F ((float)(SCREEN_XSIZE))
#define SCREEN_YSIZE_F ((float)(SCREEN_YSIZE))

#define SCREEN_XCENTER (SCREEN_XSIZE / 2)
#define SCREEN_YCENTER (SCREEN_YSIZE / 2)

#define SCREEN_XCENTER_F (SCREEN_XSIZE_F / 2.0f)
#define SCREEN_YCENTER_F (SCREEN_YSIZE_F / 2.0f)

#ifndef BASE_PATH
#define BASE_PATH ""
#endif

#define DEFAULT_FULLSCREEN false

// ============================
// ENGINE INCLUDES
// ============================

#if RETRO_USE_ORIGINAL_CODE
#include "d3d.h"
#include "dinput.h"
#include <FreeImage.h>
#include "DX7/ddutil.h"
#include "DX7/texture.h"
#include "resource.h"
#elif RETRO_USE_SDL3
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#elif RETRO_USE_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif RETRO_USE_SDL1
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#include <cdmath3d.h>
#include <coldet.h>

#include "Collision.hpp"
#include "Drawing.hpp"
#include "Ini.hpp"
#include "Reader.hpp"
#include "Input.hpp"
#include "Level.hpp"
#include "Level.hpp"
#include "Math.hpp"
#include "ModAPI.hpp"
#include "Player.hpp"
#include "String.hpp"
#include "Text.hpp"
#include "TitleScreen.hpp"
#include "Userdata.hpp"

// ============================
// ENGINE VARIABLES
// ============================

extern unsigned long long Frequency;
extern unsigned long long FrameCurrentTicks;
extern unsigned long long FrameNextTicks;
extern unsigned long long FrameLastTicks;

extern int FrameTicks;
extern double FrameSecondsPerTick;
extern double FrameDeltaTime;

#if RETRO_USE_ORIGINAL_CODE
extern MSG Message;
extern bool UseQueryCounter;
#else
extern SDL_Event SDLEvent;
#endif

extern int QuitMessage;

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

// ============================
// ENGINE FUNCTIONS
// ============================

bool CreateMWindow();

#if RETRO_USE_ORIGINAL_CODE
LRESULT CALLBACK MWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MDialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void UpdateWindowRect(HWND hWnd);
void ResetWindow(HWND hWnd);
#else
void ProcessEvents();
#endif

void ProcessMainGame();
void SetGameMode(int mode);
void PauseCheck();
void ProcessPlayerInput();
void ProcessDebugMode();
void ProcessPlayerMovement();
void ProcessObjects();
void ProcessTime();

#endif // !RETROSONICENGINE_H