#ifndef INPUT_H
#define INPUT_H

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
#elif RETRO_USE_SDL2
extern const Uint8 *keys;
#endif

bool InitInputDevice();
void ReleaseInputDevice();

void EnableInput();
void DisableInput();

void CheckInput(InputData *input);
void CheckKeyPress(InputData *input, byte start, byte end);

#endif // !INPUT_H