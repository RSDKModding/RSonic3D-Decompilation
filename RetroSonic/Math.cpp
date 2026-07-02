#include "RetroEngine.hpp"

double Cos(float value) { return cos(value); }
double Sin(float value) { return sin(value); }
double ASin(float value) { return asin(value); }
double ATan(float value) { return atan(value); }
double Sqrt(float value) { return sqrt(value); }

void IdentityMatrix(Matrix3D *dest)
{
    memset(dest, 0, sizeof(*dest));
    dest->m[0][0] = 1.0f;
    dest->m[1][1] = 1.0f;
    dest->m[2][2] = 1.0f;
    dest->m[3][3] = 1.0f;
}

void MatrixMultiply(Matrix3D *dest, Matrix3D *matrix)
{
    Matrix3D temp;
    memset(&temp, 0, sizeof(temp));

    for (ushort i = 0; i < 4; ++i) {
        for (ushort j = 0; j < 4; ++j) {
            for (ushort k = 0; k < 4; ++k) {
                temp.m[i][j] = dest->m[i][k] * matrix->m[k][j] + temp.m[i][j];
            }
        }
    }

    memcpy(dest, &temp, sizeof(*dest));
}

void MatrixPerspective(Matrix3D *dest, float w, float h, float zn, float zf)
{
    if (fabs(zf - zn) >= 0.01f && fabs(Sin(w * 0.5f)) >= 0.01f) {
        memset(dest, 0, sizeof(*dest));
        dest->m[0][0]   = Cos(w * 0.5f) / Sin(w * 0.5f) * h;
        dest->m[1][1]   = Cos(w * 0.5f) / Sin(w * 0.5f);
        dest->m[2][2]   = zf / (zf - zn);
        dest->m[2][3]   = 1.0f;
        dest->m[3][2]   = -dest->m[2][2] * zn;
    }
}

void MatrixInverse(Matrix3D *dest, Matrix3D *matrix)
{
    fabs(matrix->m[3][3] - 1.0f);
    if (fabs(matrix->m[0][3]) <= 0.001f && fabs(matrix->m[1][3]) <= 0.001f)
        fabs(matrix->m[2][3]);

    float det = 0.0f;
    det = matrix->m[0][0] * (matrix->m[1][1] * matrix->m[2][2] - matrix->m[1][2] * matrix->m[2][1]);
    det -= matrix->m[0][1] * (matrix->m[1][0] * matrix->m[2][2] - matrix->m[1][2] * matrix->m[2][0]);
    det += matrix->m[0][2] * (matrix->m[1][0] * matrix->m[2][1] - matrix->m[1][1] * matrix->m[2][0]);

    det = (1.0f / det);

    dest->m[0][0] = (matrix->m[1][1] * matrix->m[2][2] - matrix->m[1][2] * matrix->m[2][1]) * det;
    dest->m[0][1] = -(matrix->m[0][1] * matrix->m[2][2] - matrix->m[0][2] * matrix->m[2][1]) * det;
    dest->m[0][2] = (matrix->m[0][1] * matrix->m[1][2] - matrix->m[0][2] * matrix->m[1][1]) * det;
    dest->m[0][3] = 0.0f;

    dest->m[1][0] = -(matrix->m[1][0] * matrix->m[2][2] - matrix->m[1][2] * matrix->m[2][0]) * det;
    dest->m[1][1] = (matrix->m[0][0] * matrix->m[2][2] - matrix->m[0][2] * matrix->m[2][0]) * det;
    dest->m[1][2] = -(matrix->m[0][0] * matrix->m[1][2] - matrix->m[0][2] * matrix->m[1][0]) * det;
    dest->m[1][3] = 0.0f;

    dest->m[2][0] = (matrix->m[1][0] * matrix->m[2][1] - matrix->m[1][1] * matrix->m[2][0]) * det;
    dest->m[2][1] = -(matrix->m[0][0] * matrix->m[2][1] - matrix->m[0][1] * matrix->m[2][0]) * det;
    dest->m[2][2] = (matrix->m[0][0] * matrix->m[1][1] - matrix->m[0][1] * matrix->m[1][0]) * det;
    dest->m[2][3] = 0.0f;

    dest->m[3][0] = -(matrix->m[3][0] * dest->m[0][0] + matrix->m[3][1] * dest->m[1][0] + matrix->m[3][2] * dest->m[2][0]);
    dest->m[3][1] = -(matrix->m[3][0] * dest->m[0][1] + matrix->m[3][1] * dest->m[1][1] + matrix->m[3][2] * dest->m[2][1]);
    dest->m[3][2] = -(matrix->m[3][0] * dest->m[0][2] + matrix->m[3][1] * dest->m[1][2] + matrix->m[3][2] * dest->m[2][2]);
    dest->m[3][3] = 1.0f;
}