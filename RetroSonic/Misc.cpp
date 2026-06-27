#include "RetroEngine.hpp"

void LoadCharacterSurface(LPCSTR lpString2, char id)
{
    char name[64];
    lstrcpyA(name, "Data/Characters/");
    lstrcatA(name, lpString2);

    if (surfaceCharacters[id] != NULL)
        surfaceCharacters[id]->Release();

    surfaceCharacters[id] = CreateTexture(D3DDevice, name);

    if (!id)
        DDLoadBitmap(name, 0);
}

void SonicModel_405A18(int32_t frameID)
{
    for (int32_t i = 0; i < sonicAni.frames[frameID].vertexCount; ++i) {
        int32_t v = sonicAni.frames[frameID].vertexIDs[i];

        sonicTMF.vertices[v].sx = matSonicMdl.m[0][0] * sonicTMF2.vertices[v].sx + matSonicMdl.m[1][0] * sonicTMF2.vertices[v].sy
                                  + matSonicMdl.m[2][0] * sonicTMF2.vertices[v].sz + matSonicMdl.m[3][0];

        sonicTMF.vertices[v].sy = matSonicMdl.m[0][1] * sonicTMF2.vertices[v].sx + matSonicMdl.m[1][1] * sonicTMF2.vertices[v].sy
                                  + matSonicMdl.m[2][1] * sonicTMF2.vertices[v].sz + matSonicMdl.m[3][1];

        sonicTMF.vertices[v].sz = matSonicMdl.m[0][2] * sonicTMF2.vertices[v].sx + matSonicMdl.m[1][2] * sonicTMF2.vertices[v].sy
                                  + matSonicMdl.m[2][2] * sonicTMF2.vertices[v].sz + matSonicMdl.m[3][2];
    }
}

// TODO: WRONG STRUCT?
void SonicModel_405B7B(int32_t frameID)
{
    int v1; // [esp+0h] [ebp-8h]
    int i;  // [esp+4h] [ebp-4h]

    for (i = 0; i < sonicAni.frames[frameID].vertexCount; ++i) {
        v1                        = sonicAni.frames[frameID].vertexIDs[i];
        sonicTMF.vertices[v1].rhw = matSonicMdl.m[0][0] * sonicTMF2.vertices[v1].rhw + matSonicMdl.m[1][0] * *(float *)&sonicTMF2.vertices[v1].color
                                    + matSonicMdl.m[2][0] * *(float *)&sonicTMF2.vertices[v1].specular + matSonicMdl.m[3][0];
        *(float *)&sonicTMF.vertices[v1].color = matSonicMdl.m[0][1] * sonicTMF2.vertices[v1].rhw
                                                 + matSonicMdl.m[1][1] * *(float *)&sonicTMF2.vertices[v1].color
                                                 + matSonicMdl.m[2][1] * *(float *)&sonicTMF2.vertices[v1].specular + matSonicMdl.m[3][1];
        *(float *)&sonicTMF.vertices[v1].specular = matSonicMdl.m[0][2] * sonicTMF2.vertices[v1].rhw
                                                    + matSonicMdl.m[1][2] * *(float *)&sonicTMF2.vertices[v1].color
                                                    + matSonicMdl.m[2][2] * *(float *)&sonicTMF2.vertices[v1].specular + matSonicMdl.m[3][2];
    }
}

void SonicModel_405CE2(uint8_t a1, float a2)
{
    if (a1 != sonicAni.field_BFAB) {
        sonicAni.field_BFAB = a1;
        sonicAni.field_BFB0 = 0;
        sonicAni.field_BFAE = 0;
        if (sonicAni.field_BFAA == 4) {
            sonicAni.field_BFAA = sonicAni.field_BFAB;
            sonicAni.field_BFAC = 0;
            data_4C9D4C         = 0.0f;
        }
    }

    if (a1 == 2) {
        sonicAni.array_AB90[2].field_201 = (__int64)(a2 * 128.0);
    }
    else if (a1 == 4) {
        data_4C9D4C = data_4C9D4C - a2 * 0.30000001f;
        if (data_4C9D4C < 0.0f)
            data_4C9D4C = 3.1415927f + 3.1415927f;
        sonicAni.array_AB90[4].field_201 = (__int64)(a2 * 128.0);
    }
}

void ProcessPlayerAnimationLMC()
{
    int v0;       // edx
    float a1;     // [esp+0h] [ebp-30h]
    float a1_4;   // [esp+4h] [ebp-2Ch]
    float a2;     // [esp+8h] [ebp-28h]
    int i;        // [esp+1Ch] [ebp-14h]
    int j;        // [esp+1Ch] [ebp-14h]
    float v6;     // [esp+20h] [ebp-10h]
    char v7;      // [esp+24h] [ebp-Ch]
    float value;  // [esp+28h] [ebp-8h]
    float valuea; // [esp+28h] [ebp-8h]
    float valueb; // [esp+28h] [ebp-8h]
    int k;        // [esp+2Ch] [ebp-4h]
    int l;        // [esp+2Ch] [ebp-4h]

    v7 = 0;
    if (sonicAni.field_BFAA == sonicAni.field_BFAB)
        v0 = sonicAni.field_BFAA;
    else
        v0 = sonicAni.field_BFAB;
    sonicAni.field_BFB0 += sonicAni.array_AB90[v0].field_201;
    if (sonicAni.field_BFB0 > 0xEFu) {
        sonicAni.field_BFB0 -= 240;
        if (++sonicAni.field_BFAC >= (int)sonicAni.array_AB90[sonicAni.field_BFAA].count)
            sonicAni.field_BFAC = sonicAni.array_AB90[sonicAni.field_BFAA].field_200;
        if (sonicAni.field_BFAA != sonicAni.field_BFAB) {
            sonicAni.field_BFAA = sonicAni.field_BFAB;
            sonicAni.field_BFAC = 0;
        }
        sonicAni.field_BFAE = sonicAni.field_BFAC + 1;
        if (sonicAni.field_BFAE >= (int)sonicAni.array_AB90[sonicAni.field_BFAA].count)
            sonicAni.field_BFAE = sonicAni.array_AB90[sonicAni.field_BFAA].field_200;
    }
    v6 = (double)sonicAni.field_BFB0 / 240.0;
    for (i = 0; i < 36; ++i) {
        memcpy(&matrix_4C8990[i], &matWorld, sizeof(D3DMATRIX));
        value = (1.0 - v6) * sonicAni.frames[i].rotX[sonicAni.array_AB90[sonicAni.field_BFAA].array_2[sonicAni.field_BFAC]]
                + v6 * sonicAni.frames[i].rotX[sonicAni.array_AB90[sonicAni.field_BFAB].array_2[sonicAni.field_BFAE]];
        MatrixRotateX_4C9DB0(value);
        MatrixMultiply(&matrix_4C8990[i], &matrixBackgroundTransform);
        valuea = (1.0 - v6) * sonicAni.frames[i].rotY[sonicAni.array_AB90[sonicAni.field_BFAA].array_2[sonicAni.field_BFAC]]
                 + v6 * sonicAni.frames[i].rotY[sonicAni.array_AB90[sonicAni.field_BFAB].array_2[sonicAni.field_BFAE]];
        MatrixRotateY_4C9DB0(valuea);
        MatrixMultiply(&matrix_4C8990[i], &matrixBackgroundTransform);
        valueb = (1.0 - v6) * sonicAni.frames[i].rotZ[sonicAni.array_AB90[sonicAni.field_BFAA].array_2[sonicAni.field_BFAC]]
                 + v6 * sonicAni.frames[i].rotZ[sonicAni.array_AB90[sonicAni.field_BFAB].array_2[sonicAni.field_BFAE]];
        MatrixRotateZ_4C9DB0(valueb);
        MatrixMultiply(&matrix_4C8990[i], &matrixBackgroundTransform);
        memcpy(&matrix_4C9290[i], &matWorld, sizeof(D3DMATRIX));
        a2   = -sonicAni.frames[i].position.z;
        a1_4 = -sonicAni.frames[i].position.y;
        a1   = -sonicAni.frames[i].position.x;
        Matrix_408B0B(a1, a1_4, a2);
        MatrixMultiply(&matrix_4C9290[i], &matrixBackgroundTransform);
        MatrixMultiply(&matrix_4C9290[i], &matrix_4C8990[i]);
        Matrix_408B0B(sonicAni.frames[i].position.x, sonicAni.frames[i].position.y, sonicAni.frames[i].position.z);
        MatrixMultiply(&matrix_4C9290[i], &matrixBackgroundTransform);
    }
    for (j = 0; j < sonicAni.field_BFA8; ++j) {
        if (v7) {
            if (sonicAni.frameIDs[j] == 255) {
                v7 = 0;
            }
            else if (sonicAni.frameIDs[j] == 254) {
                v7 = 1;
            }
            else {
                memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                for (k = j; sonicAni.frameIDs[k] < 0xFEu; ++k) MatrixMultiply(&matSonicMdl, &matrix_4C9290[sonicAni.frameIDs[k]]);
                SonicModel_405A18(sonicAni.frameIDs[j]);
                memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                for (l = j; sonicAni.frameIDs[l] < 254u; ++l) MatrixMultiply(&matSonicMdl, &matrix_4C8990[sonicAni.frameIDs[l]]);
                SonicModel_405B7B(sonicAni.frameIDs[j]);
            }
        }
        else if (sonicAni.frameIDs[j] == 254) {
            v7 = 1;
        }
        else if (sonicAni.frameIDs[j] == 255) {
            v7 = 0;
        }
        else {
            memcpy(&matSonicMdl, &matrix_4C9290[sonicAni.frameIDs[j]], sizeof(matSonicMdl));
            SonicModel_405A18(sonicAni.frameIDs[j]);
            memcpy(&matSonicMdl, &matrix_4C8990[sonicAni.frameIDs[j]], sizeof(matSonicMdl));
            SonicModel_405B7B(sonicAni.frameIDs[j]);
        }
    }
}

void MightBeSonicAnim_406432()
{
    float v0; // [esp+0h] [ebp-28h]
    float a2; // [esp+4h] [ebp-24h]
    float v2; // [esp+8h] [ebp-20h]
    int j;    // [esp+18h] [ebp-10h]
    int k;    // [esp+18h] [ebp-10h]
    int i;    // [esp+1Ch] [ebp-Ch]
    char v6;  // [esp+20h] [ebp-8h]
    int m;    // [esp+24h] [ebp-4h]
    int n;    // [esp+24h] [ebp-4h]

    v6 = 0;
    for (i = 0; i < 36; ++i) {
        for (j = 0; j < 36; ++j) {
            memcpy(&matrix_4C8990[j], &matWorld, sizeof(D3DMATRIX));
            MatrixRotateX_4C9DB0(sonicAni.frames[j].rotX[i]);

            MatrixMultiply(&matrix_4C8990[j], &matrixBackgroundTransform);
            MatrixRotateY_4C9DB0(sonicAni.frames[j].rotY[i]);

            MatrixMultiply(&matrix_4C8990[j], &matrixBackgroundTransform);
            MatrixRotateZ_4C9DB0(sonicAni.frames[j].rotZ[i]);

            MatrixMultiply(&matrix_4C8990[j], &matrixBackgroundTransform);
            memcpy(&matrix_4C9290[j], &matWorld, sizeof(D3DMATRIX));

            Matrix_408B0B(-sonicAni.frames[j].position.x, -sonicAni.frames[j].position.y, -sonicAni.frames[j].position.z);
            MatrixMultiply(&matrix_4C9290[j], &matrixBackgroundTransform);
            MatrixMultiply(&matrix_4C9290[j], &matrix_4C8990[j]);
            Matrix_408B0B(sonicAni.frames[j].position.x, sonicAni.frames[j].position.y, sonicAni.frames[j].position.z);
            MatrixMultiply(&matrix_4C9290[j], &matrixBackgroundTransform);
        }

        for (k = 0; k < sonicAni.field_BFA8; ++k) {
            if (v6) {
                if (sonicAni.frameIDs[k] == 255) {
                    v6 = 0;
                }
                else if (sonicAni.frameIDs[k] == 254) {
                    v6 = 1;
                }
                else {
                    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                    for (m = k; sonicAni.frameIDs[m] < 254u; ++m) MatrixMultiply(&matSonicMdl, &matrix_4C9290[sonicAni.frameIDs[m]]);
                    memcpy(&matrix_47A790[k][i], &matSonicMdl, sizeof(matrix_47A790[k][i]));
                    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
                    for (n = k; sonicAni.frameIDs[n] < 254u; ++n) MatrixMultiply(&matSonicMdl, &matrix_4C8990[sonicAni.frameIDs[n]]);
                    memcpy(&array_42C590[k][i], &matSonicMdl, sizeof(array_42C590[k][i]));
                }
            }
            else if (sonicAni.frameIDs[k] == 254) {
                v6 = 1;
            }
            else if (sonicAni.frameIDs[k] == 255) {
                v6 = 0;
            }
            else {
                memcpy(&matSonicMdl, &matrix_4C9290[sonicAni.frameIDs[k]], sizeof(matSonicMdl));
                SonicModel_405A18(sonicAni.frameIDs[k]);
                memcpy(&matSonicMdl, &matrix_4C8990[sonicAni.frameIDs[k]], sizeof(matSonicMdl));
                SonicModel_405B7B(sonicAni.frameIDs[k]);
            }
        }
    }
}

// Seems to move the world camera
void Matrix_40398C(D3DMATRIX *m, Vector3D *a2, Vector3D *a3, Vector3D *a4)
{
    Vector3D v_3_2 = (*a3) - (*a2);
    float value    = v_3_2.Magnitude();

    v_3_2 /= value;

    float v24   = (*a4) * v_3_2;
    Vector3D v4 = v24 * v_3_2;

    Vector3D v_3_1 = (*a4) - v4;
    value          = v_3_1.Magnitude();

    if (value < 0.000001f) {
        Vector3D v9 = v_3_2.y * v_3_2;
        Vector3D v6 = Vector3D(0.0f, 1.0f, 0.0f);
        v_3_1       = v6 - v9;
        value       = v_3_1.Magnitude();

        if (value < 0.000001f) {
            Vector3D v10 = v_3_2.z * v_3_2;
            Vector3D v8  = Vector3D(0.0f, 0.0f, 1.0f);
            v_3_1        = v8 - v10;
            value        = v_3_1.Magnitude();
        }
    }

    v_3_1 /= value;
    Vector3D v_3_0 = CrossProduct(v_3_1, v_3_2);

    IdentityMatrix(m);

    m->m[0][0] = v_3_0.x;
    m->m[0][1] = v_3_1.x;
    m->m[0][2] = v_3_2.x;
    m->m[1][0] = v_3_0.y;
    m->m[1][1] = v_3_1.y;
    m->m[1][2] = v_3_2.y;
    m->m[2][0] = v_3_0.z;
    m->m[2][1] = v_3_1.z;
    m->m[2][2] = v_3_2.z;

    MatrixInverse(&matrix_4C9C90, m);

    m->m[3][0] = -((*a2) * v_3_0);
    m->m[3][1] = -((*a2) * v_3_1);
    m->m[3][2] = -((*a2) * v_3_2);
}

void Matrix_408779(float a1)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[1][1] = cos(a1);
    matrixBackgroundTransform.m[1][2] = sin(a1);
    matrixBackgroundTransform.m[2][1] = -sin(a1);
    matrixBackgroundTransform.m[2][2] = cos(a1);
    IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matrixBackgroundTransform);
}

void Matrix_40880F(float a1)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[0][0] = cos(a1);
    matrixBackgroundTransform.m[0][2] = -sin(a1);
    matrixBackgroundTransform.m[2][0] = sin(a1);
    matrixBackgroundTransform.m[2][2] = cos(a1);
    IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matrixBackgroundTransform);
}

void Matrix_4088A5(float a1)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[0][0] = cos(a1);
    matrixBackgroundTransform.m[0][1] = sin(a1);
    matrixBackgroundTransform.m[1][0] = -sin(a1);
    matrixBackgroundTransform.m[1][1] = cos(a1);
    IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matrixBackgroundTransform);
}

void Matrix_40893B(int a1, int a2, int a3)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[3][0] = a1;
    matrixBackgroundTransform.m[3][1] = a2;
    matrixBackgroundTransform.m[3][2] = a3;
    matrixBackgroundTransform.m[3][3] = 1.0;
    IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matrixBackgroundTransform);
}

void MatrixRotateX_4C9DB0(float value)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[1][1] = cos(value);
    matrixBackgroundTransform.m[1][2] = sin(value);
    matrixBackgroundTransform.m[2][1] = -sin(value);
    matrixBackgroundTransform.m[2][2] = cos(value);
}

void MatrixRotateY_4C9DB0(float value)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[0][0] = cos(value);
    matrixBackgroundTransform.m[0][2] = -sin(value);
    matrixBackgroundTransform.m[2][0] = sin(value);
    matrixBackgroundTransform.m[2][2] = cos(value);
}

void MatrixRotateZ_4C9DB0(float value)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[0][0] = cos(value);
    matrixBackgroundTransform.m[0][1] = sin(value);
    matrixBackgroundTransform.m[1][0] = -sin(value);
    matrixBackgroundTransform.m[1][1] = cos(value);
}

void Matrix_408B0B(float a1, float a2, float a3)
{
    memcpy(&matrixBackgroundTransform, &matWorld, sizeof(matrixBackgroundTransform));
    matrixBackgroundTransform.m[3][0] = a1;
    matrixBackgroundTransform.m[3][1] = a2;
    matrixBackgroundTransform.m[3][2] = a3;
    matrixBackgroundTransform.m[3][3] = 1.0f;
}

void MultiplyMatrix_4C9B90_4C9BD0() { MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform); }

//
//
//

void DrawModelShadow(float x, float y, float z, float p_4, float a5, float a6, float a7)
{
    //
}

void DrawModelSonic(float x, float y, float z, float rotation)
{
    IDirect3DDevice7_SetTexture(D3DDevice, 0, surfaceSonic);
    IDirect3DDevice7_SetRenderState(D3DDevice, D3DRENDERSTATE_SPECULARENABLE, TRUE);
    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));

    if (sonicAni.field_BFAA == 4) {
        if (sonicAni.field_BFAC) {
            MatrixRotateY_4C9DB0(rotation);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            Matrix_408B0B(x, y, z);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, ballTMF.vertices, ballTMF.numVertices,
                                                  ballTMF.indices, ballTMF.numIndices, 0);
        }
        else {
            Matrix_408B0B(0.0f, -5.4000001f, 0.0f);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            MatrixRotateX_4C9DB0(data_4C9D4C);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            Matrix_408B0B(0.0f, 3.8f, 0.0f);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            MatrixRotateY_4C9DB0(rotation);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            Matrix_408B0B(x, y, z);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);

            IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, sonicTMF.vertices, sonicTMF.numVertices,
                                                  sonicTMF.indices, sonicTMF.numIndices, 0);
            material_420520.diffuse.a = 0.25f;
            IDirect3DDevice7_SetMaterial(D3DDevice, &material_420520);
            memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
            Matrix_40880F(rotation);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            Matrix_408B0B(x, y, z);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, ballTMF.vertices, ballTMF.numVertices,
                                                  ballTMF.indices, ballTMF.numIndices, 0);
            material_420520.diffuse.a = 1.0f;
            IDirect3DDevice7_SetMaterial(D3DDevice, &material_420520);
        }
    }
    else {
        MatrixRotateY_4C9DB0(rotation);
        MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
        MatrixRotateX_4C9DB0(data_4C9D64);
        MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
        MatrixRotateZ_4C9DB0(data_4C9D68);
        MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
        Matrix_408B0B(x, y, z);
        MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
        IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matSonicMdl);

        IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, sonicTMF.vertices, sonicTMF.numVertices, sonicTMF.indices,
                                              sonicTMF.numIndices, 0);
    }

    IDirect3DDevice7_SetRenderState(D3DDevice, D3DRENDERSTATE_SPECULARENABLE, FALSE);
    DrawModelShadow(x, y, z, -30.0f, 2.5f, 3.0f, rotation);
}

void DrawObjectModelID(int object, float x, float y, float z, float ry, float rx, float rz)
{
    D3DLVERTEX vertices[4];
    memset(&vertices, 0, sizeof(vertices));

    uint16_t indices[6] = { 0, 1, 2, 1, 3, 2 };

    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));

    MatrixRotateY_4C9DB0(ry);
    MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);

    MatrixRotateX_4C9DB0(rx);
    MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);

    MatrixRotateZ_4C9DB0(rz);
    MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);

    Matrix_408B0B(x, y, z);
    MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);

    switch (object) {
        case 0:
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            D3DDevice->SetTexture(0, stageObjectTextures[object]);
            D3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 196608);
            D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (0x002 | 0x010 | 0x100), stageObjectModel[object].vertices,
                                            stageObjectModel[object].numVertices, stageObjectModel[object].indices,
                                            stageObjectModel[object].numIndices, 0);
            D3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
            break;

        case 2: {
            vertices[0] = D3DLVERTEX({ -2.0f, 2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.05f);
            vertices[1] = D3DLVERTEX({ 2.0f, 2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.05f);
            vertices[2] = D3DLVERTEX({ -2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.99f);
            vertices[3] = D3DLVERTEX({ 2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.99f);

            memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
            MatrixRotateZ_4C9DB0(rz);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            MatrixMultiply(&matSonicMdl, &matrix_4C9C90);
            Matrix_408B0B(x, y, z);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);

            D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            D3DDevice->SetTexture(0, sparkleTexture);
            D3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, 0);
            D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (0x002 | 0x020 | 0x040 | 0x080 | 0x100), vertices, 4, indices, 6, 0);
            D3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, 1);
            break;
        }

        default:
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            D3DDevice->SetTexture(0, stageObjectTextures[object]);
            D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (0x002 | 0x010 | 0x100), stageObjectModel[object].vertices,
                                            stageObjectModel[object].numVertices, stageObjectModel[object].indices,
                                            stageObjectModel[object].numIndices, 0);
            break;
    }
}

//
//

void Matrix_40812C(float *a1, float *a2, float *a3)
{
    float v3; // [esp+8h] [ebp-Ch]
    float v4; // [esp+Ch] [ebp-8h]
    float v5; // [esp+10h] [ebp-4h]

    v4 = *a1;
    v5 = *a2;
    v3 = *a3;
    memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
    MatrixRotateX_4C9DB0(data_4C9D5C);
    MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
    MatrixRotateZ_4C9DB0(data_4C9D60);
    MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
    *a1 = matSonicMdl.m[0][0] * v4 + matSonicMdl.m[1][0] * v5 + matSonicMdl.m[2][0] * v3 + matSonicMdl.m[3][0];
    *a2 = matSonicMdl.m[0][1] * v4 + matSonicMdl.m[1][1] * v5 + matSonicMdl.m[2][1] * v3 + matSonicMdl.m[3][1];
    *a3 = matSonicMdl.m[0][2] * v4 + matSonicMdl.m[1][2] * v5 + matSonicMdl.m[2][2] * v3 + matSonicMdl.m[3][2];
}

void Unknown_408222()
{
    data_4C9D5C = 0.0f;
    data_4C9D60 = 0.0f;
}

// TODO: Move this to Drawing.cpp, clean
void DrawText_1(const char *chars, int32_t strLength, int32_t xOffset, int32_t yOffset, char flag)
{
    D3DLVERTEX vertices[80];
    memset(vertices, 0, sizeof(vertices));

    uint16_t indices[120];

    float_t texU, texV, texUR, texVB;
    float_t originX = (float_t)(xOffset - 160);
    float_t baseY   = (120 - yOffset) * 0.0083330004f;
    D3DCOLOR color  = (flag == 8) ? 0xFFFFFF00 : 0xBFFFFFFF;

    for (int32_t i = 0; i < strLength; ++i) {
        float_t x = (10.0f * i + originX) * 0.0062500001f;

        if (chars[i]) {
            texU  = ((chars[i] - 1) % 6) * 0.1640625f;
            texV  = ((chars[i] - 1) / 6) * 0.1640625f;
            texUR = texU + 0.1640625f;
            texVB = texV + 0.1640625f;
        }
        else {
            texU  = 0.0f;
            texV  = 0.0f;
            texUR = 0.0f;
            texVB = 0.0f;
        }

        D3DVECTOR position_0(x, baseY, 1.0f);
        D3DVECTOR position_1(x + 0.079999998f, baseY, 1.0f);
        D3DVECTOR position_2(x, baseY - 0.083333001f, 1.0f);
        D3DVECTOR position_3(x + 0.079999998f, baseY - 0.083333001f, 1.0f);

        vertices[4 * i + 0] = D3DLVERTEX(position_0, color, 0.0f, texU, texV);
        vertices[4 * i + 1] = D3DLVERTEX(position_1, color, 0.0f, texUR, texV);
        vertices[4 * i + 2] = D3DLVERTEX(position_2, color, 0.0f, texU, texVB);
        vertices[4 * i + 3] = D3DLVERTEX(position_3, color, 0.0f, texUR, texVB);

        indices[6 * i + 0] = (4 * i) + 0;
        indices[6 * i + 1] = (4 * i) + 1;
        indices[6 * i + 2] = (4 * i) + 2;
        indices[6 * i + 3] = (4 * i) + 1;
        indices[6 * i + 4] = (4 * i) + 3;
        indices[6 * i + 5] = (4 * i) + 2;
    }

    int32_t numVertices = 4 * strLength;
    int32_t numIndices  = 6 * strLength;

    IDirect3DDevice7_BeginScene(D3DDevice);
    IDirect3DDevice7_SetTexture(D3DDevice, 0, surfaceMText);
    IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices, numVertices, indices, numIndices, 0);
    IDirect3DDevice7_EndScene(D3DDevice);
}

void DrawText_2(const char *chars, int32_t length, int32_t xOffset, int32_t yOffset)
{
    D3DLVERTEX vertices[80];
    memset(vertices, 0, sizeof(vertices));

    uint16_t indices[120];

    float_t texU, texV, texUR, texVB;
    float_t originX = (float_t)(xOffset - 160);
    float_t baseY   = (120 - yOffset) * 0.0083330004f;
    float_t color   = -1.9999999f;
    float_t normalY = NAN;

    for (int32_t i = 0; i < length; ++i) {
        float_t x = (10.0f * i + originX) * 0.0062500001f;

        if (chars[i]) {
            int32_t index = (unsigned char)chars[i] - 1;
            texU          = (index % 6) * 0.1640625f;
            texV          = (index / 6) * 0.1640625f;
            texUR         = texU + 0.1640625f;
            texVB         = texV + 0.1640625f;
        }
        else {
            texU  = 0.0f;
            texV  = 0.0f;
            texUR = 0.0f;
            texVB = 0.0f;
        }

        D3DVECTOR position_0(x, baseY, 1.0f);
        D3DVECTOR position_1(x + 0.079999998f, baseY, 1.0f);
        D3DVECTOR position_2(x, baseY - 0.083333001f, 1.0f);
        D3DVECTOR position_3(x + 0.079999998f, baseY - 0.083333001f, 1.0f);

        vertices[(4 * i) + 0] = D3DLVERTEX(position_0, (i == length - 1) ? color : normalY, 0.0f, texU, texV);
        vertices[(4 * i) + 1] = D3DLVERTEX(position_1, (i == length - 1) ? color : normalY, 0.0f, texUR, texV);
        vertices[(4 * i) + 2] = D3DLVERTEX(position_2, (i == length - 1) ? color : normalY, 0.0f, texU, texVB);
        vertices[(4 * i) + 3] = D3DLVERTEX(position_3, (i == length - 1) ? color : normalY, 0.0f, texUR, texVB);

        indices[(6 * i) + 0] = (4 * i) + 0;
        indices[(6 * i) + 1] = (4 * i) + 1;
        indices[(6 * i) + 2] = (4 * i) + 2;
        indices[(6 * i) + 3] = (4 * i) + 1;
        indices[(6 * i) + 4] = (4 * i) + 3;
        indices[(6 * i) + 5] = (4 * i) + 2;
    }

    int32_t numVertices = 4 * length;
    int32_t numIndices  = 6 * length;

    IDirect3DDevice7_BeginScene(D3DDevice);
    IDirect3DDevice7_SetTexture(D3DDevice, 0, surfaceMText);
    IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices, numVertices, indices, numIndices, 0);
    IDirect3DDevice7_EndScene(D3DDevice);
}

void DrawText_3(const char *chars, int32_t length, int32_t xOffset, int32_t yOffset, int32_t a5, uint8_t a6, uint8_t a7)
{
    D3DLVERTEX vertices[80];
    memset(vertices, 0, sizeof(vertices));

    uint16_t indices[120];

    float_t texU, texV, texUR, texVB;
    float_t originX = (float_t)(xOffset - 160);
    float_t baseY   = (120 - yOffset) * 0.0083330004f;
    float_t colorA, colorB;
    float_t normalY = NAN;

    if (a6 <= 8) {
        if (a7 <= 8) {
            colorA = ((8 - a6) * 0.0625f) * 255.0f;
            colorB = ((8 - a7) * 0.0625f) * 255.0f;
        }
        else {
            colorA = ((16 - a7) * 0.0625f) * 255.0f;
            colorB = 2.3509886e-38f;
        }
    }
    else {
        colorA = 2.3509886e-38f;
        colorB = ((16 - a6) * 0.0625f) * 255.0f;
    }

    if (a6 < 16 && a7 < 16) {
        for (int32_t i = 0; i < length; ++i) {
            float_t x = (10.0f * i + originX) * 0.0062500001f;

            if (chars[i]) {
                int32_t index = (unsigned char)chars[i] - 1;
                texU          = (index % 6) * 0.1640625f;
                texV          = (index / 6) * 0.1640625f;
                texUR         = texU + 0.1640625f;
                texVB         = texV + 0.1640625f;
            }
            else {
                texU  = 0.0f;
                texV  = 0.0f;
                texUR = 0.0f;
                texVB = 0.0f;
            }

            D3DVECTOR position_0(x, baseY, 1.0f);
            D3DVECTOR position_1(x + 0.079999998f, baseY, 1.0f);
            D3DVECTOR position_2(x, baseY - 0.083333001f, 1.0f);
            D3DVECTOR position_3(x + 0.079999998f, baseY - 0.083333001f, 1.0f);

            vertices[(4 * i) + 0] = D3DLVERTEX(position_0, colorA, 0.0f, texU, texV);
            vertices[(4 * i) + 1] = D3DLVERTEX(position_1, colorA, 0.0f, texUR, texV);
            vertices[(4 * i) + 2] = D3DLVERTEX(position_2, normalY, 0.0f, texU, texVB);
            vertices[(4 * i) + 3] = D3DLVERTEX(position_3, normalY, 0.0f, texUR, texVB);

            indices[(6 * i) + 0] = (4 * i) + 0;
            indices[(6 * i) + 1] = (4 * i) + 1;
            indices[(6 * i) + 2] = (4 * i) + 2;
            indices[(6 * i) + 3] = (4 * i) + 1;
            indices[(6 * i) + 4] = (4 * i) + 3;
            indices[(6 * i) + 5] = (4 * i) + 2;
        }

        int32_t numVertices = 4 * length;
        int32_t numIndices  = 6 * length;

        IDirect3DDevice7_BeginScene(D3DDevice);
        IDirect3DDevice7_SetTexture(D3DDevice, 0, surfaceMText);
        IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices, numVertices, indices, numIndices, 0);
        IDirect3DDevice7_EndScene(D3DDevice);
    }
}
