#ifndef R_MISC_H
#define R_MISC_H

void Matrix_40398C(D3DMATRIX *matrix, Vector3D &a2, Vector3D &a3, Vector3D &a4);

void WorldMatrixTranslateXYZ(float x, float y, float z);
void WorldMatrixRotateZ(float value);
void WorldMatrixRotateY(float value);
void WorldMatrixRotateX(float value);

void WorldMatrixSetTranslateXYZ(float x, float y, float z);
void WorldMatrixSetRotateX(float x);
void WorldMatrixSetRotateY(float y);
void WorldMatrixSetRotateZ(float z);

enum ObjectID {
    OBJECT_ID_RING,
    OBJECT_ID_SPRING,
    OBJECT_ID_RING_SPARKLE,
};

void DrawModelShadow(float x, float y, float z, float magnitude, float sx, float sy, float ry);
void DrawModelSonic(float x, float y, float z, float a4);
void DrawObjectModelID(int object, float x, float y, float z, float ry, float rx, float rz);

void PlayerRotationPhysics(float *a1, float *a2, float *a3);
void ResetPlayerRotation();

#endif // !R_MISC_H