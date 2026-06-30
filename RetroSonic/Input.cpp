#include "RetroEngine.hpp"

int InputEnabled = true;
InputData InputPress;

bool InitInputDevice() { return InputDevice::Init(); }
void ReleaseInputDevice() { InputDevice::Release(); }

void EnableInput()
{
    if (InputEnabled == false) {
        InputDevice::Enable();
        InputEnabled = true;
    }
}

void DisableInput()
{
    if (InputEnabled == true) {
        InputDevice::Disable();
        InputEnabled = false;
    }
}

void CheckInput(InputData *input) { InputDevice::CheckInput(input); }
void CheckKeyPress(InputData *input, byte start, byte end) { InputDevice::CheckKeyPress(input, start, end); }