#ifndef PLAYER_H
#define PLAYER_H

enum CharacterIDs {
    CHARACTER_SONIC_TAILS,
    CHARACTER_SONIC,
    CHARACTER_TAILS,
    CHARACTER_KNUCKLES_TAILS,
    CHARACTER_KNUCKLES,
    CHARACTER_COUNT,
};

enum Gravity {
    GRAVITY_GROUND,
    GRAVITY_AIR,
};

struct PlayerObject {
    Vector3D position;
    Vector3D velocity;
    Vector3D collideDir;
    float f_0x24; // some acceleration/speed thing
    int f_0x28; // seems rotation related. maybe rotation turn value
    int f_0x2C; // seems to be a max value for f_0x28, max turn speed?
    double f_0x30; // y rotation? result of turning?
    byte b0;
    byte b1;
    byte b2;
    byte b3;
    byte f_0x38; // state?
    byte b5;
    byte b6;
    sbyte f_0x3B; // disable inputs/movement, still allows camera turn. maybe controlMode?
    int field_3C;
    int field_40;
    int gravity;
    int up;
    int jumpPress;
    int f_0x50; // pressed Z
    byte gap_54[96];
};

extern Matrix3D MatrixSonicModel;
extern Matrix3D MatrixSonicNodeRotation[36];
extern Matrix3D MatrixSonicNodeTransform[36];

extern float PlayerTargetRotationZ;
extern float PlayerTargetRotationX;

extern float PlayerRotationZ;
extern float PlayerRotationX;

extern float PlayerPrevRotationZ;
extern float PlayerPrevRotationX;

extern int PlayerRotationTimerX;
extern int PlayerRotationTimerZ;

extern float data_4C9D4C;

extern byte PNumber;
extern PlayerObject Player[2];

void ProcessPlayerCamera();

void LoadPlayerGfx(const char *textureName, sbyte characterID);

void HandleSonicVertexPositions(int frameID);
void HandleSonicVertexNormals(int frameID);
void SonicModel_405CE2(byte a1, float a2);
void ProcessPlayerAnimationLMC();
void MightBeSonicAnim_406432();

#endif // !PLAYER_H