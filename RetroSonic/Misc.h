#ifndef R_MISC_H
#define R_MISC_H

void LoadCharacterSurface(LPCSTR lpString2, char id);

void SonicModel_405A18(int32_t frameID);
void SonicModel_405B7B(int32_t frameID);
void SonicModel_405CE2(uint8_t a1, float a2);
void ProcessPlayerAnimationLMC();
void MightBeSonicAnim_406432();

void Matrix_40398C(D3DMATRIX *m, Vector3D *a2, Vector3D *a3, Vector3D *a4);
void Matrix_408779(float a1);
void Matrix_40880F(float a1);
void Matrix_4088A5(float a1);
void Matrix_40893B(int a1, int a2, int a3);
void MatrixRotateX_4C9DB0(float value);
void MatrixRotateY_4C9DB0(float value);
void MatrixRotateZ_4C9DB0(float value);
void Matrix_408B0B(float a1, float a2, float a3);

void MultiplyMatrix_4C9B90_4C9BD0();

enum ObjectID {
    OBJECT_ID_RING,
    OBJECT_ID_SPRING,
    OBJECT_ID_RING_SPARKLE,
};

void DrawModelShadow(float x, float y, float z, float p_4, float a5, float a6, float a7);
void DrawModelSonic(float x, float y, float z, float a4);
void DrawObjectModelID(int object, float x, float y, float z, float ry, float rx, float rz);

void Matrix_40812C(float *a1, float *a2, float *a3);
void Unknown_408222();

void DrawText_1(const char *chars, int32_t strLength, int32_t xOffset, int32_t yOffset, char flag);
void DrawText_2(const char *chars, int32_t length, int32_t xOffset, int32_t yOffset);
void DrawText_3(const char *chars, int32_t length, int32_t xOffset, int32_t yOffset, int32_t a5, uint8_t a6, uint8_t a7);

#endif // !R_MISC_H