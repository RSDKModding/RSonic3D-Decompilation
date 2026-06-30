#include "RetroEngine.hpp"

using namespace RenderDevice; // temp

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

    MatrixInverse(&MatrixInversed, matrix);

    matrix->m[3][0] = param1 * variable5;
    matrix->m[3][1] = param1 * variable4;
    matrix->m[3][2] = param1 * variable1;

    matrix->m[3][0] = -matrix->m[3][0];
    matrix->m[3][1] = -matrix->m[3][1];
    matrix->m[3][2] = -matrix->m[3][2];
}

void WorldMatrixTranslateXYZ(float x, float y, float z)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[3][0] = x;
    MatrixWorld.m[3][1] = y;
    MatrixWorld.m[3][2] = z;
    MatrixWorld.m[3][3] = 1.0f;
}

void WorldMatrixRotateX(float value)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[0][0] = cos(value);
    MatrixWorld.m[0][1] = sin(value);
    MatrixWorld.m[1][0] = -sin(value);
    MatrixWorld.m[1][1] = cos(value);
}

void WorldMatrixRotateY(float value)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[0][0] = cos(value);
    MatrixWorld.m[0][2] = -sin(value);
    MatrixWorld.m[2][0] = sin(value);
    MatrixWorld.m[2][2] = cos(value);
}

void WorldMatrixRotateZ(float x)
{
    memcpy(&MatrixWorld, &MatrixIdentity, sizeof(MatrixWorld));
    MatrixWorld.m[1][1] = cos(x);
    MatrixWorld.m[1][2] = sin(x);
    MatrixWorld.m[2][1] = -sin(x);
    MatrixWorld.m[2][2] = cos(x);
}

void WorldMatrixSetTranslateXYZ(float x, float y, float z)
{
    WorldMatrixTranslateXYZ(x, y, z);
    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void WorldMatrixSetRotateX(float z)
{
    WorldMatrixRotateX(z);
    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void WorldMatrixSetRotateY(float y)
{
    WorldMatrixRotateY(y);
    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void WorldMatrixSetRotateZ(float x)
{
    WorldMatrixRotateZ(x);
    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixWorld);
}

void DrawModelShadow(float x, float y, float z, float magnitude, float sx, float sy, float ry)
{
    Vector3D origin    = { x, y + 1.0f, z };
    Vector3D direction = { 0.0f, 1.0f, 0.0f };

    float point[3];
    float colist[9];

    float c = (x - LevelModel.startX) / 50.0f;
    float r = (z - LevelModel.startZ) / 50.0f;

    int minRow = CLAMP(r - 2, 0, LevelModel.rows);
    int maxRow = CLAMP(r + 2, 0, LevelModel.rows);

    int minCol = CLAMP(c - 2, 0, LevelModel.columns);
    int maxCol = CLAMP(c + 2, 0, LevelModel.columns);

    Collided = false;

    D3DLVERTEX vertices[4];
    ZeroMemory(vertices, sizeof(vertices));

    ushort indices[] = { 0, 1, 2, 1, 3, 2 };

    float alpha = 0.5f;
    for (int row = minRow; row < maxRow; ++row) {
        for (int col = minCol; col < maxCol; ++col) {
            if (LCollision[row][col]->rayCollision(&origin[0], &direction[0], false, 0.0f, magnitude)) {
                LCollision[row][col]->getCollisionPoint(point);
                x = point[0];
                y = point[1] + sy * 0.01f;
                z = point[2];

                LCollision[row][col]->getCollidingTriangles(colist, NULL);
                Collided = true;
            }
        }
    }

    if (Collided == true) {
        origin.y = (origin.y - 2.0f) - y;
        origin.x = magnitude * -0.75f;
        origin.z = magnitude * -0.5f;

        Vector3D ab = { colist[3] - colist[0], colist[4] - colist[1], colist[5] - colist[2] };
        Vector3D ac = { colist[6] - colist[0], colist[7] - colist[1], colist[8] - colist[2] };

        if (origin.y > origin.x)
            alpha = 0.5f - (origin.y - origin.x) / (-magnitude - origin.x) * 0.5f;

        if (origin.y > origin.z) {
            sx -= (origin.y - origin.z) / (-magnitude - origin.z) * sx;
            sy -= (origin.y - origin.z) / (-magnitude - origin.z) * sy;
        }

        Vector3D normal = CrossProduct(ab, ac).Normalized();
        D3DCOLOR color  = TO_ARGB_F(alpha, 255, 255, 255);

        vertices[0] = { { -sx, 0.0f, sy }, color, 0, 0.05f, 0.05f };
        vertices[1] = { { sx, 0.0f, sy }, color, 0, 0.99f, 0.05f };
        vertices[2] = { { -sx, 0.0f, -sy }, color, 0, 0.05f, 0.99f };
        vertices[3] = { { sx, 0.0f, -sy }, color, 0, 0.99f, 0.99f };

        MatrixSonicModel = MatrixIdentity;
        WorldMatrixRotateY(ry);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixRotateZ(ASin(normal.z));
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixRotateX(-ASin(normal.x));
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixTranslateXYZ(x, y, z);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

        RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
        RenderDevice::SetTexture(0, surfaceShadow);
        RenderDevice::DrawIndexed(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
    }
}

void DrawModelSonic(float x, float y, float z, float rotation)
{
    RenderDevice::SetTexture(0, surfaceSonic);
    IDirect3DDevice7_SetRenderState(D3DDevice, D3DRENDERSTATE_SPECULARENABLE, TRUE);
    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    if (SonicAni.field_BFAA == 4) {
        if (SonicAni.field_BFAC) {
            WorldMatrixRotateY(rotation);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, BallMdl.vertices, BallMdl.numVertices, BallMdl.indices, BallMdl.numIndices);
        }
        else {
            WorldMatrixTranslateXYZ(0.0f, -5.4000001f, 0.0f);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            WorldMatrixRotateZ(data_4C9D4C);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            WorldMatrixTranslateXYZ(0.0f, 3.8f, 0.0f);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            WorldMatrixRotateY(rotation);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, SonicMdl.vertices, SonicMdl.numVertices, SonicMdl.indices, SonicMdl.numIndices);
            material_420520.diffuse.a = 0.25f;
            RenderDevice::SetMaterial(&material_420520);
            memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
            WorldMatrixSetRotateY(rotation);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, BallMdl.vertices, BallMdl.numVertices, BallMdl.indices, BallMdl.numIndices);
            material_420520.diffuse.a = 1.0f;
            RenderDevice::SetMaterial(&material_420520);
        }
    }
    else {
        WorldMatrixRotateY(rotation);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixRotateZ(PlayerRotationZ);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixRotateX(PlayerRotationX);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        WorldMatrixTranslateXYZ(x, y, z);
        MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
        RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
        RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, SonicMdl.vertices, SonicMdl.numVertices, SonicMdl.indices, SonicMdl.numIndices);
    }

    IDirect3DDevice7_SetRenderState(D3DDevice, D3DRENDERSTATE_SPECULARENABLE, FALSE);
    DrawModelShadow(x, y, z, -30.0f, 2.5f, 3.0f, rotation);
}

void DrawObjectModelID(int object, float x, float y, float z, float ry, float rx, float rz)
{
    D3DLVERTEX vertices[4];
    memset(&vertices, 0, sizeof(vertices));

    uint16_t indices[6] = { 0, 1, 2, 1, 3, 2 };

    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    WorldMatrixRotateY(ry);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixRotateZ(rx);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixRotateX(rz);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixTranslateXYZ(x, y, z);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    switch (object) {
        case 0:
            RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            RenderDevice::SetTexture(0, stageObjectTextures[object]);
            RenderDevice::SetTextureStageState(0, TEXTURE_STATE_TEXCOORDINDEX, 196608);
            RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, StageObjMdl[object].vertices, StageObjMdl[object].numVertices, StageObjMdl[object].indices,
                                      StageObjMdl[object].numIndices);
            RenderDevice::SetTextureStageState(0, TEXTURE_STATE_TEXCOORDINDEX, 0);
            break;

        case 2: {
            vertices[0] = D3DLVERTEX({ -2.0f, 2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.05f);
            vertices[1] = D3DLVERTEX({ 2.0f, 2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.05f);
            vertices[2] = D3DLVERTEX({ -2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.99f);
            vertices[3] = D3DLVERTEX({ 2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.99f);

            memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));
            WorldMatrixRotateX(rz);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);
            MatrixMultiply(&MatrixSonicModel, &MatrixInversed);
            WorldMatrixTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

            RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            RenderDevice::SetTexture(0, sparkleTexture);
            RenderDevice::SetRenderState(RENDER_STATE_LIGHTING, 0);
            RenderDevice::DrawIndexed(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
            RenderDevice::SetRenderState(RENDER_STATE_LIGHTING, 1);
            break;
        }

        default:
            RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
            RenderDevice::SetTexture(0, stageObjectTextures[object]);
            RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, StageObjMdl[object].vertices, StageObjMdl[object].numVertices, StageObjMdl[object].indices,
                                      StageObjMdl[object].numIndices);
            break;
    }
}

void PlayerRotationPhysics(float *x, float *y, float *z)
{
    float sx = *x;
    float sy = *y;
    float sz = *z;

    memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel));

    WorldMatrixRotateZ(PlayerTargetRotationZ);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    WorldMatrixRotateX(PlayerTargetRotationX);
    MatrixMultiply(&MatrixSonicModel, &MatrixWorld);

    *x = MatrixSonicModel.m[0][0] * sx + MatrixSonicModel.m[1][0] * sy + MatrixSonicModel.m[2][0] * sz + MatrixSonicModel.m[3][0];
    *y = MatrixSonicModel.m[0][1] * sx + MatrixSonicModel.m[1][1] * sy + MatrixSonicModel.m[2][1] * sz + MatrixSonicModel.m[3][1];
    *z = MatrixSonicModel.m[0][2] * sx + MatrixSonicModel.m[1][2] * sy + MatrixSonicModel.m[2][2] * sz + MatrixSonicModel.m[3][2];
}

void ResetPlayerRotation()
{
    PlayerTargetRotationZ = 0.0f;
    PlayerTargetRotationX = 0.0f;
}