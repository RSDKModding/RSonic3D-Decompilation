#ifndef MATH_H
#define MATH_H

#define RSDK_PI   (3.1415927)
#define RSDK_PI_H (RSDK_PI / 2.0)

#define TO_RADIAN(degree) ((degree) * (RSDK_PI / 180.0f))
#define CLAMP(value, minimum, maximum) (((value) < (minimum)) ? (minimum) : (((value) > (maximum)) ? (maximum) : (value)))

#define UNUSED(x) (void)x
#define MEM_ZERO(x, s) memset(x, 0, s)

double Cos(float value);
double Sin(float value);
double ASin(float value);
double ATan(float value);
double Sqrt(float value);
double Fabs(float value);

void IdentityMatrix(Matrix3D *matrix);
void MatrixMultiply(Matrix3D *dst, Matrix3D *src);
void MatrixPerspective(Matrix3D *matrix, float w, float h, float zn, float zf);
void MatrixInverse(Matrix3D *dst, Matrix3D *src);

#endif // !MATH_H