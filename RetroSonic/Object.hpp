#ifndef OBJECT_H
#define OBJECT_H

#define ENTITY_COUNT (0x44C)

struct Object {
    byte type;
    byte propertyValue;
    Vector3D position;
    Vector3D unused;
    int valueA;
    int valueB;
    int valueC;
    int valueD;
    int valueE;
    int valueF;
    int priority;
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
void CreateObject(byte type, byte propertyValue, float x, float y, float z);
void DrawObjectModel(int object, float x, float y, float z, float ry, float rx, float rz);
void DrawModelShadow(float x, float y, float z, float magnitude, float sx, float sy, float ry);

#endif // !OBJECT_H