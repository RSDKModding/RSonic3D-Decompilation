#ifndef INPUT_H
#define INPUT_H

#if RETRO_USE_ORIGINAL_CODE
#define INPUT_SCANCODE_LEFT   DIK_LEFT
#define INPUT_SCANCODE_RIGHT  DIK_RIGHT
#define INPUT_SCANCODE_UP     DIK_UP
#define INPUT_SCANCODE_DOWN   DIK_DOWN
#define INPUT_SCANCODE_RETURN DIK_RETURN
#define INPUT_SCANCODE_LCTRL  DIK_LCONTROL
#define INPUT_SCANCODE_LSHIFT DIK_LSHIFT
#define INPUT_SCANCODE_Z      DIK_Z
#define INPUT_SCANCODE_X      DIK_X
#elif RETRO_USE_SDL3 || RETRO_USE_SDL2
#define INPUT_SCANCODE_LEFT   SDL_SCANCODE_LEFT
#define INPUT_SCANCODE_RIGHT  SDL_SCANCODE_RIGHT
#define INPUT_SCANCODE_UP     SDL_SCANCODE_UP
#define INPUT_SCANCODE_DOWN   SDL_SCANCODE_DOWN
#define INPUT_SCANCODE_RETURN SDL_SCANCODE_RETURN
#define INPUT_SCANCODE_LCTRL  SDL_SCANCODE_LCTRL
#define INPUT_SCANCODE_LSHIFT SDL_SCANCODE_LSHIFT
#define INPUT_SCANCODE_Z      SDL_SCANCODE_Z
#define INPUT_SCANCODE_X      SDL_SCANCODE_X
#elif RETRO_USE_SDL1
#define INPUT_SCANCODE_LEFT   SDLK_LEFT
#define INPUT_SCANCODE_RIGHT  SDLK_RIGHT
#define INPUT_SCANCODE_UP     SDLK_UP
#define INPUT_SCANCODE_DOWN   SDLK_DOWN
#define INPUT_SCANCODE_RETURN SDLK_RETURN
#define INPUT_SCANCODE_LCTRL  SDLK_LCTRL
#define INPUT_SCANCODE_LSHIFT SDLK_LSHIFT
#define INPUT_SCANCODE_Z      SDLK_z
#define INPUT_SCANCODE_X      SDLK_x
#endif

enum InputButtons {
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_START,
    INPUT_LCONTROL,
    INPUT_LSHIFT,
    INPUT_Z,
    INPUT_X,
    INPUT_ONCE,
};

struct InputData {
    int left;
    int right;
    int up;
    int down;
    int start;
    int control;
    int shift;
    int Z;
    int X;
};

extern int InputEnabled;
extern InputData InputPress;

#if RETRO_USE_ORIGINAL_CODE
extern IDirectInputA *DirectInput;
extern IDirectInputDeviceA *DirectInputDevice;

extern char keys[0x100];
#elif RETRO_USE_SDL3
extern const bool *keys;
#elif RETRO_USE_SDL2 || RETRO_USE_SDL1
extern const Uint8 *keys;
#endif

bool InitInputDevice();
void ReleaseInputDevice();

void EnableInput();
void DisableInput();

void CheckInput(InputData *input);
void CheckKeyPress(InputData *input, byte start, byte end);

#endif // !INPUT_H