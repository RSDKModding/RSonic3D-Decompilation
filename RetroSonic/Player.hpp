#ifndef ANIMATION_H
#define ANIMATION_H

struct PlayerObject {
    Vector3D position;
    Vector3D velocity;
    Vector3D f_0x18;
    float_t f_0x24;
    int32_t f_0x28;
    int32_t f_0x2C;
    double_t f_0x30;
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t f_0x38;
    uint8_t b5;
    uint8_t b6;
    uint8_t f_0x3B;
    uint8_t gap_3C[4];
    int32_t flagA;
    int32_t f_0x44;
    int32_t f_0x48;
    int32_t f_0x4C;
    int32_t f_0x50;
    uint8_t gap_54[96];
};

extern byte PNumber;
extern PlayerObject Player[2];

void ProcessPlayerInput();
void ProcessPlayerCamera();
void ProcessPlayerMovement();

#endif // !ANIMATION_H