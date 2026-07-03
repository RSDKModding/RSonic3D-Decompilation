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

enum PlayerAnimationIDs {
    ANI_STOPPED,
    ANI_WAITING,
    ANI_WALKING,
    ANI_RUNNING,
    ANI_JUMPING,
    ANI_TITLESCREEN,
};

enum PlayerStates {
    STATE_GROUND,
    STATE_AIR,
    STATE_STATIC,
};

struct PlayerObject {
    Vector3D position;
    Vector3D velocity;
    Vector3D collisionPos;
    float speed;
    int angle;
    int targetAngle;
    float rotationY;
    int unused1;
    byte state;
    byte unused2;
    byte disableControl;
    int unused3;
    int unused4;
    int gravity;
    int up;
    int jumpPress;
    int z;
    byte unused5;
    int unused6;
    int unused7;
    int unused8;
    int unused9;
    int unused10;
    int unused11;
    int unused12;
    int unused13;
    int unused14;
    int unused15;
    int unused16;
    int unused17;
    int unused18;
    int unused19;
    int unused20;
    int unused21;
    int unused22;
    int unused23;
    int unused24;
    int unused25;
    int unused26;
    int unused27;
    int unused28;
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

extern float PlayerJumpRotationX;

extern int PlayerRotationTimerX;
extern int PlayerRotationTimerZ;

extern byte PNumber;
extern PlayerObject Player[2];

void ProcessPlayerCamera();

void LoadPlayerGfx(const char *textureName, sbyte characterID);

void HandleSonicVertexPositions(int frameID);
void HandleSonicVertexNormals(int frameID);
void SetPlayerAnimationID(byte animation, float speed);
void ProcessPlayerAnimationLMC();
void MightBeSonicAnim_406432();

#endif // !PLAYER_H