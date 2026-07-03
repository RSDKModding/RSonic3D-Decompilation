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