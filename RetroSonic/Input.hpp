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

extern int InputDeviceState;
extern int InputEnabled;
extern char keys[0x100];

extern InputData InputPress;
extern LPDIRECTINPUTDEVICEA DirectInputDevice;
extern LPDIRECTINPUTA DirectInput;

int InitInputDevice(HWND hWnd, HINSTANCE hInst);
void ReleaseInputDevice();

void EnableInput();
void DisableInput();

void CheckInput(InputData *input);
void CheckKeyPress(InputData *input, byte start, byte end);

#endif // !INPUT_H