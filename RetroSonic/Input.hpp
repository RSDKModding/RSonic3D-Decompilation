#ifndef INPUT_H
#define INPUT_H

#if RETRO_USE_ORIGINAL_CODE
#define KEYBOARD_SCANCODE_LEFT   DIK_LEFT
#define KEYBOARD_SCANCODE_RIGHT  DIK_RIGHT
#define KEYBOARD_SCANCODE_UP     DIK_UP
#define KEYBOARD_SCANCODE_DOWN   DIK_DOWN
#define KEYBOARD_SCANCODE_RETURN DIK_RETURN
#define KEYBOARD_SCANCODE_LCTRL  DIK_LCONTROL
#define KEYBOARD_SCANCODE_LSHIFT DIK_LSHIFT
#define KEYBOARD_SCANCODE_A      DIK_Z
#define KEYBOARD_SCANCODE_B      DIK_X
#else
extern int KEYBOARD_SCANCODE_LEFT;
extern int KEYBOARD_SCANCODE_RIGHT;
extern int KEYBOARD_SCANCODE_UP;
extern int KEYBOARD_SCANCODE_DOWN;
extern int KEYBOARD_SCANCODE_RETURN;
extern int KEYBOARD_SCANCODE_LCTRL;
extern int KEYBOARD_SCANCODE_LSHIFT;
extern int KEYBOARD_SCANCODE_A;
extern int KEYBOARD_SCANCODE_B;
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

extern bool InputEnabled;
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