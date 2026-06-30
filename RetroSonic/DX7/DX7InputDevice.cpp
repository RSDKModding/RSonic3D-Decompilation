#include "RetroEngine.hpp"

IDirectInputA *InputDevice::DirectInput;
IDirectInputDeviceA *InputDevice::DirectInputDevice;
char InputDevice::keys[0x100];

bool InputDevice::Init()
{
    if (FAILED(DirectInputCreate(RenderDevice::HInst, DIRECTINPUT_VERSION, &DirectInput, NULL)))
        return false;

    if (FAILED(DirectInput->CreateDevice(GUID_SysKeyboard, &DirectInputDevice, 0)))
        return false;

    DirectInputDevice->SetDataFormat(&c_dfDIKeyboard);
    DirectInputDevice->SetCooperativeLevel(RenderDevice::HWnd, 0x00000002 | 0x00000004);
    DirectInputDevice->Acquire();
    DirectInputDevice->GetDeviceState(sizeof(keys), keys);
    return true;
}

void InputDevice::Release()
{
    DirectInputDevice->Unacquire();
    DirectInputDevice->Release();
    DirectInputDevice = NULL;

    DirectInput->Release();
    DirectInput = NULL;
}

void InputDevice::Enable() { DirectInputDevice->Acquire(); }

void InputDevice::Disable() { DirectInputDevice->Unacquire(); }

void InputDevice::CheckInput(InputData *input)
{
    DirectInputDevice->GetDeviceState(sizeof(keys), keys);

    input->left    = keys[DIK_LEFT] < 0;
    input->right   = keys[DIK_RIGHT] < 0;
    input->up      = keys[DIK_UP] < 0;
    input->down    = keys[DIK_DOWN] < 0;
    input->start   = keys[DIK_RETURN] < 0;
    input->control = keys[DIK_LCONTROL] < 0;
    input->shift   = keys[DIK_LSHIFT] < 0;
    input->Z       = keys[DIK_Z] < 0;
    input->X       = keys[DIK_X] < 0;
}

void InputDevice::CheckKeyPress(InputData *input, byte start, byte end)
{
    DirectInputDevice->GetDeviceState(sizeof(keys), keys);

    sbyte scancodes[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (int i = start; i < end; ++i) {
        switch (scancodes[i]) {
            case 1:
                if (keys[DIK_LEFT] & 0x80) {
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
                if (keys[DIK_RIGHT] & 0x80) {
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
                if (keys[DIK_UP] & 0x80) {
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
                if (keys[DIK_DOWN] & 0x80) {
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
                if (keys[DIK_RETURN] & 0x80) {
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
                if (keys[DIK_LCONTROL] & 0x80) {
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
                if (keys[DIK_LSHIFT] & 0x80) {
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
                if (keys[DIK_Z] & 0x80) {
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
                if (keys[DIK_X] & 0x80) {
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