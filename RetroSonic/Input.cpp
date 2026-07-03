#include "RetroEngine.hpp"

int InputEnabled = true;
InputData InputPress;

#if RETRO_USE_ORIGINAL_CODE
IDirectInputA *DirectInput;
IDirectInputDeviceA *DirectInputDevice;

char keys[0x100];
#elif RETRO_USE_SDL3
const bool *keys = NULL;
#elif RETRO_USE_SDL2 || RETRO_USE_SDL1
const Uint8 *keys = NULL;
#endif

bool InitInputDevice()
{
#if RETRO_USE_ORIGINAL_CODE
    if (FAILED(DirectInputCreate(HInst, DIRECTINPUT_VERSION, &DirectInput, NULL)))
        return false;

    if (FAILED(DirectInput->CreateDevice(GUID_SysKeyboard, &DirectInputDevice, 0)))
        return false;

    DirectInputDevice->SetDataFormat(&c_dfDIKeyboard);
    DirectInputDevice->SetCooperativeLevel(HWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    DirectInputDevice->Acquire();
    DirectInputDevice->GetDeviceState(sizeof(keys), keys);
    return true;
#elif RETRO_USE_SDL3 || RETRO_USE_SDL2
    keys = SDL_GetKeyboardState(NULL);
    return keys != NULL;
#elif RETRO_USE_SDL1
    keys = SDL_GetKeyState(NULL);
    return keys != NULL;
#endif
}

void ReleaseInputDevice()
{
#if RETRO_USE_ORIGINAL_CODE
    DirectInputDevice->Unacquire();
    DirectInputDevice->Release();
    DirectInputDevice = NULL;

    DirectInput->Release();
    DirectInput = NULL;
#else
    keys = NULL;
#endif
}

void EnableInput()
{
    if (InputEnabled == false) {
#if RETRO_USE_ORIGINAL_CODE
        DirectInputDevice->Acquire();
#endif
        InputEnabled = true;
    }
}

void DisableInput()
{
    if (InputEnabled == true) {
#if RETRO_USE_ORIGINAL_CODE
        DirectInputDevice->Unacquire();
#endif
        InputEnabled = false;
    }
}

void CheckInput(InputData *input)
{
#if RETRO_USE_ORIGINAL_CODE
    DirectInputDevice->GetDeviceState(sizeof(keys), keys);

    input->left    = keys[INPUT_SCANCODE_LEFT] < 0;
    input->right   = keys[INPUT_SCANCODE_RIGHT] < 0;
    input->up      = keys[INPUT_SCANCODE_UP] < 0;
    input->down    = keys[INPUT_SCANCODE_DOWN] < 0;
    input->start   = keys[INPUT_SCANCODE_RETURN] < 0;
    input->control = keys[INPUT_SCANCODE_LCTRL] < 0;
    input->shift   = keys[INPUT_SCANCODE_LSHIFT] < 0;
    input->Z       = keys[INPUT_SCANCODE_Z] < 0;
    input->X       = keys[INPUT_SCANCODE_X] < 0;
#else
    SDL_PumpEvents();
    if (keys == NULL)
        return;

    input->left    = keys[INPUT_SCANCODE_LEFT] != 0;
    input->right   = keys[INPUT_SCANCODE_RIGHT] != 0;
    input->up      = keys[INPUT_SCANCODE_UP] != 0;
    input->down    = keys[INPUT_SCANCODE_DOWN] != 0;
    input->start   = keys[INPUT_SCANCODE_RETURN] != 0;
    input->control = keys[INPUT_SCANCODE_LCTRL] != 0;
    input->shift   = keys[INPUT_SCANCODE_LSHIFT] != 0;
    input->Z       = keys[INPUT_SCANCODE_Z] != 0;
    input->X       = keys[INPUT_SCANCODE_X] != 0;
#endif
}

void CheckKeyPress(InputData *input, byte start, byte end)
{
#if RETRO_USE_ORIGINAL_CODE
    DirectInputDevice->GetDeviceState(sizeof(keys), keys);
#else
    SDL_PumpEvents();
    if (keys == NULL)
        return;
#endif

    sbyte ids[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (int i = start; i < end; ++i) {
        switch (ids[i]) {
            case 1:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_LEFT] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_LEFT] != 0) {
#endif
                    if (InputPress.left) {
                        input->left = false;
                    }
                    else {
                        InputPress.left = true;
                        input->left     = true;
                    }
                }
                else {
                    input->left     = false;
                    InputPress.left = false;
                }
                break;

            case 2:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_RIGHT] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_RIGHT] != 0) {
#endif
                    if (InputPress.right) {
                        input->right = false;
                    }
                    else {
                        InputPress.right = true;
                        input->right     = true;
                    }
                }
                else {
                    input->right     = false;
                    InputPress.right = false;
                }
                break;

            case 3:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_UP] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_UP] != 0) {
#endif
                    if (InputPress.up) {
                        input->up = false;
                    }
                    else {
                        InputPress.up = true;
                        input->up     = true;
                    }
                }
                else {
                    input->up     = false;
                    InputPress.up = false;
                }
                break;

            case 4:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_DOWN] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_DOWN] != 0) {
#endif
                    if (InputPress.down) {
                        input->down = false;
                    }
                    else {
                        InputPress.down = true;
                        input->down     = true;
                    }
                }
                else {
                    input->down     = false;
                    InputPress.down = false;
                }
                break;

            case 5:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_RETURN] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_RETURN] != 0) {
#endif
                    if (InputPress.start) {
                        input->start = false;
                    }
                    else {
                        InputPress.start = true;
                        input->start     = true;
                    }
                }
                else {
                    input->start     = false;
                    InputPress.start = false;
                }
                break;

            case 6:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_LCTRL] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_LCTRL] != 0) {
#endif
                    if (InputPress.control) {
                        input->control = false;
                    }
                    else {
                        InputPress.control = true;
                        input->control     = true;
                    }
                }
                else {
                    input->control     = false;
                    InputPress.control = false;
                }
                break;

            case 7:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_LSHIFT] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_LSHIFT] != 0) {
#endif
                    if (InputPress.shift) {
                        input->shift = false;
                    }
                    else {
                        InputPress.shift = true;
                        input->shift     = true;
                    }
                }
                else {
                    input->shift     = false;
                    InputPress.shift = false;
                }
                break;

            case 8:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_Z] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_Z] != 0) {
#endif
                    if (InputPress.Z) {
                        input->Z = false;
                    }
                    else {
                        InputPress.Z = true;
                        input->Z     = true;
                    }
                }
                else {
                    input->Z     = false;
                    InputPress.Z = false;
                }
                break;

            case 9:
#if RETRO_USE_ORIGINAL_CODE
                if (keys[INPUT_SCANCODE_X] & 0x80) {
#else
                if (keys[INPUT_SCANCODE_X] != 0) {
#endif
                    if (InputPress.X) {
                        input->X = false;
                    }
                    else {
                        InputPress.X = true;
                        input->X     = true;
                    }
                }
                else {
                    input->X     = false;
                    InputPress.X = false;
                }
                break;

            default: continue;
        }
    }
}