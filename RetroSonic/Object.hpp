#ifndef OBJECT_H
#define OBJECT_H

#define ENTITY_COUNT (0x44C)

struct Object {
    byte type;
    byte unused1;
    Vector3D position;
    int unused2;
    int unused3;
    int unused4;
    int timer;
    int unused5;
    int unused6;
    int unused7;
    int unused8;
    int unused9;
    int enabled;
};

enum ObjectTypes {
    OBJ_TYPE_BLANKOBJECT = 0,
    OBJ_TYPE_RING        = 1,
    OBJ_TYPE_RINGSPARKLE = 3,
    OBJ_TYPE_SPRING      = 4,
};

enum ObjectModels {
    OBJ_MODEL_RING,
    OBJ_MODEL_SPRING,
    OBJ_MODEL_RINGSPARKLE,
};

enum Gravity {
    GRAVITY_GROUND,
    GRAVITY_AIR,
};

extern int ObjectLoop;
extern int TempObjectPos;
extern Object LevelObjects[ENTITY_COUNT];

void ProcessObjects();
void DrawObjects();
void CreateObject(byte type, byte unused, float x, float y, float z);
void DrawObjectModel(int object, float x, float y, float z, float ry, float rx, float rz);
void DrawModelShadow(float x, float y, float z, float magnitude, float sx, float sy, float ry);

#endif // !OBJECT_H