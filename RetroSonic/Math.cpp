#include "RetroEngine.hpp"

double Cos(float value) { return cos(value); }
double Sin(float value) { return sin(value); }
double ASin(float value) { return asin(value); }
double ATan(float value) { return atan(value); }
double Sqrt(float value) { return sqrt(value); }
double Fabs(float value) { return fabs(value); }

void IdentityMatrix(Matrix3D *dest)
{
    *dest = Matrix3D::Identity;
}

void MatrixMultiply(Matrix3D *dest, Matrix3D *matrix)
{
    (*dest) *= (*matrix);
}

void MatrixPerspective(Matrix3D *dest, float w, float h, float zn, float zf)
{
    if (Fabs(zf - zn) >= 0.01f && Fabs(Sin(w * 0.5f)) >= 0.01f) {
        IdentityMatrix(dest);

        dest->m[0][0] = Cos(w * 0.5f) / Sin(w * 0.5f) * h;
        dest->m[1][1] = Cos(w * 0.5f) / Sin(w * 0.5f) * 1.0f;
        dest->m[2][2] = zf / (zf - zn);
        dest->m[2][3] = 1.0f;
        dest->m[3][2] = -dest->m[2][2] * zn;
    }
}

void MatrixInverse(Matrix3D *dest, Matrix3D *matrix)
{
    *dest = matrix->Inverse();
}

void MatrixWorldTranslateXYZ(float x, float y, float z)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[3][0] = x;
    MatrixWorld.m[3][1] = y;
    MatrixWorld.m[3][2] = z;
    MatrixWorld.m[3][3] = 1.0f;
}

void MatrixWorldRotateX(float value)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[0][0] = cos(value);
    MatrixWorld.m[0][1] = sin(value);
    MatrixWorld.m[1][0] = -sin(value);
    MatrixWorld.m[1][1] = cos(value);
}

void MatrixWorldRotateY(float value)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[0][0] = cos(value);
    MatrixWorld.m[0][2] = -sin(value);
    MatrixWorld.m[2][0] = sin(value);
    MatrixWorld.m[2][2] = cos(value);
}

void MatrixWorldRotateZ(float x)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[1][1] = cos(x);
    MatrixWorld.m[1][2] = sin(x);
    MatrixWorld.m[2][1] = -sin(x);
    MatrixWorld.m[2][2] = cos(x);
}

void MatrixWorldSetTranslateXYZ(float x, float y, float z)
{
    MatrixWorldTranslateXYZ(x, y, z);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void MatrixWorldSetRotateX(float z)
{
    MatrixWorldRotateX(z);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void MatrixWorldSetRotateY(float y)
{
    MatrixWorldRotateY(y);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void MatrixWorldSetRotateZ(float x)
{
    MatrixWorldRotateZ(x);
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}