#include "RetroEngine.hpp"

// Seems to move the world camera
void Matrix_40398C(D3DMATRIX *matrix, Vector3D &param1, Vector3D &param2, Vector3D &param3)
{
    Vector3D variable1 = param2 - param1;

    float variable2 = variable1.Magnitude();
    variable1 /= variable2;

    float variable3    = param3 * variable1;
    Vector3D variable4 = param3 - (variable3 * variable1);

    if (variable2 < 0.000001f) {
        variable4 = { 0.0f, 1.0f, 0.0f };
        variable4 = variable4 - (variable1.y * variable1);

        if (variable2 < 0.000001f) {
            variable4 = { 0.0f, 0.0f, 1.0f };
            variable4 = variable4 - (variable1.z * variable1);
        }
    }

    variable4 /= variable4.Magnitude();
    Vector3D variable5 = CrossProduct(variable4, variable1);

    IdentityMatrix(matrix);

    matrix->m[0][0] = variable5.x;
    matrix->m[0][1] = variable4.x;
    matrix->m[0][2] = variable1.x;

    matrix->m[1][0] = variable5.y;
    matrix->m[1][1] = variable4.y;
    matrix->m[1][2] = variable1.y;

    matrix->m[2][0] = variable5.z;
    matrix->m[2][1] = variable4.z;
    matrix->m[2][2] = variable1.z;

    MatrixInverse(&matrix_4C9C90, matrix);

    matrix->m[3][0] = param1 * variable5;
    matrix->m[3][1] = param1 * variable4;
    matrix->m[3][2] = param1 * variable1;

    matrix->m[3][0] = -matrix->m[3][0];
    matrix->m[3][1] = -matrix->m[3][1];
    matrix->m[3][2] = -matrix->m[3][2];
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

    if (SonicAni.field_BFAA == 4) {
        if (SonicAni.field_BFAC) {
            MatrixRotateY_4C9DB0(rotation);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            Matrix_408B0B(x, y, z);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, BallMdl.vertices, BallMdl.numVertices,
                                                  BallMdl.indices, BallMdl.numIndices, 0);
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
            IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, SonicMdl.vertices, SonicMdl.numVertices,
                                                  SonicMdl.indices, SonicMdl.numIndices, 0);
            material_420520.diffuse.a = 0.25f;
            IDirect3DDevice7_SetMaterial(D3DDevice, &material_420520);
            memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl));
            Matrix_40880F(rotation);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            Matrix_408B0B(x, y, z);
            MatrixMultiply(&matSonicMdl, &matrixBackgroundTransform);
            IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matSonicMdl);
            IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, BallMdl.vertices, BallMdl.numVertices,
                                                  BallMdl.indices, BallMdl.numIndices, 0);
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

        IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, SonicMdl.vertices, SonicMdl.numVertices, SonicMdl.indices,
                                              SonicMdl.numIndices, 0);
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
            D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (0x002 | 0x010 | 0x100), StageObjMdl[object].vertices,
                                            StageObjMdl[object].numVertices, StageObjMdl[object].indices,
                                            StageObjMdl[object].numIndices, 0);
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
            D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (0x002 | 0x010 | 0x100), StageObjMdl[object].vertices,
                                            StageObjMdl[object].numVertices, StageObjMdl[object].indices,
                                            StageObjMdl[object].numIndices, 0);
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