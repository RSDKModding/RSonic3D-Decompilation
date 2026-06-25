#pragma once

#define RETRO_PI      (3.1415927)
#define RETRO_PI_H    (RETRO_PI / 2.0)
#define RETRO_DEG_RAD (RETRO_PI / 180.0)

#define TO_RADIAN(degree) ((degree) * (RETRO_PI / 180.0f))
#define TO_DEGREE(radian) ((radian) * (180.0f / RETRO_PI))

#define CLAMP(value, minimum, maximum) (((value) < (minimum)) ? (minimum) : (((value) > (maximum)) ? (maximum) : (value)))

#define UNUSED(x) (void)x

double Cos(float value);
double Sin(float value);
double ASin(float value);
double ATan(float value);
double Sqrt(float value);

void IdentityMatrix(D3DMATRIX *matrix);
void MatrixMultiply(D3DMATRIX *dst, D3DMATRIX *src);
void MatrixPerspective(D3DMATRIX *matrix, float w, float h, float zn, float zf);
void MatrixInverse(D3DMATRIX *dst, D3DMATRIX *src);