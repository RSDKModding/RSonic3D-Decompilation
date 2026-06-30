#ifndef ANIMATION_H
#define ANIMATION_H

struct PlayerObject {
    Vector3D position;
    Vector3D velocity;
    Vector3D f_0x18; // codlet collision direction?
    float_t f_0x24; // some acceleration/speed thing
    int32_t f_0x28; // seems rotation related. maybe rotation turn value
    int32_t f_0x2C; // seems to be a max value for f_0x28, max turn speed?
    double_t f_0x30; // x rotation? result of turning?
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t f_0x38; // state?
    uint8_t b5;
    uint8_t b6;
    sbyte f_0x3B; // disable inputs/movement, still allows camera turn. maybe controlMode?
    uint8_t gap_3C[4];
    int f_0x40;
    int f_0x44; // bouncing/in air, maybe gravity
    int f_0x48; // pressed up? or if the player should move forward
    int f_0x4C; // pressed lctrl/jump
    int f_0x50; // pressed Z
    uint8_t gap_54[96];
};

extern float PlayerTargetRotationZ;
extern float PlayerTargetRotationX;

extern float PlayerRotationZ;
extern float PlayerRotationX;

extern float PlayerPrevRotationZ;
extern float PlayerPrevRotationX;

extern int PlayerRotationTimerX;
extern int PlayerRotationTimerZ;

extern byte PNumber;
extern PlayerObject Player[2];

void ProcessPlayerInput();
void ProcessPlayerCamera();
void ProcessPlayerMovement();

void LoadPlayerGfx(const char *textureName, sbyte playerID);

void HandleSonicVertexPositions(int32_t frameID);
void HandleSonicVertexNormals(int32_t frameID);
void SonicModel_405CE2(uint8_t a1, float a2);
void ProcessPlayerAnimationLMC();
void MightBeSonicAnim_406432();

#endif // !ANIMATION_H