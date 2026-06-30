#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "Input.hpp"
#include <dinput.h>

namespace InputDevice
{
    extern IDirectInputA *DirectInput;
    extern IDirectInputDeviceA *DirectInputDevice;
    extern char keys[0x100];

    bool Init();
    void Release();

    void Enable();
    void Disable();

    void CheckInput(InputData *input);
    void CheckKeyPress(InputData *input, byte start, byte end);
};

#endif // !INPUT_DEVICE_H