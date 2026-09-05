#include "RetroEngine.hpp"

int ObjectLoop;
int TempObjectPos = ENTITY_COUNT - 232;
Object LevelObjects[ENTITY_COUNT];

void ProcessObjects()
{
    PlayerObject *player  = &Player[PNumber];
    PlayerObject *player1 = &Player[0];

    for (ObjectLoop = 0; ObjectLoop < ENTITY_COUNT; ++ObjectLoop) {
        Object *entity = &LevelObjects[ObjectLoop];
        if (entity->priority > 0) {
            switch (entity->type) {
                case OBJ_TYPE_RING: {
                    Vector3D position = entity->position - player1->position;
                    position.y += player1->collisionPos.y;

                    if (position.Magnitude() < 6.0f)
                        entity->type = OBJ_TYPE_RINGSPARKLE;
                    break;
                }

                case OBJ_TYPE_RINGSPARKLE: {
                    if (++entity->valueA > 15) {
                        entity->valueA = 0;
                        entity->type   = OBJ_TYPE_BLANKOBJECT;
                    }
                    break;
                }

                case OBJ_TYPE_SPRING: {
                    Vector3D position = entity->position - player1->position;
                    position.y += player1->collisionPos.y;

                    if (position.Magnitude() < 6.0f) {
                        player->velocity.y = 4.0f;
                        player->gravity    = GRAVITY_AIR;
                    }
                    break;
                }
            }
        }
    }

    AngleTimer += 0.05f;
    if (AngleTimer > RSDK_PI * 2)
        AngleTimer -= RSDK_PI * 2;
}

void DrawObjects()
{
    for (ObjectLoop = 0; ObjectLoop < ENTITY_COUNT; ++ObjectLoop) {
        Object *entity = &LevelObjects[ObjectLoop];

        float x = entity->position.x;
        float y = entity->position.y;
        float z = entity->position.z;

        if (entity->priority > 0) {
            switch (entity->type) {
                case 1:
                    DrawObjectModel(OBJ_MODEL_RING, x, y, z, AngleTimer, 0.0f, 0.0f);
                    DrawModelShadow(x, y, z, -30.0f, 2.0f, 1.0f, AngleTimer);
                    break;

                case 3:
                    switch (entity->valueA >> 2) {
                        case 0: DrawObjectModel(OBJ_MODEL_RINGSPARKLE, x, y, z, 0.0, 0.0, 0.0f); break;
                        case 1: DrawObjectModel(OBJ_MODEL_RINGSPARKLE, x, y, z, 0.0, 0.0, 1.57f); break;
                        case 2: DrawObjectModel(OBJ_MODEL_RINGSPARKLE, x, y, z, 0.0f, 0.0f, RSDK_PI); break;
                        case 3: DrawObjectModel(OBJ_MODEL_RINGSPARKLE, x, y, z, 0.0f, 0.0f, RSDK_PI + 1.57f); break;
                        default: continue;
                    }
                    break;

                case 4: DrawObjectModel(OBJ_MODEL_SPRING, x, y, z, 0.0f, 0.0f, 0.0f); break;
            }
        }
    }
}

void CreateObject(byte type, byte propertyValue, float x, float y, float z)
{
    if (LevelObjects[TempObjectPos].type != OBJ_TYPE_BLANKOBJECT) {
        if (++TempObjectPos >= ENTITY_COUNT)
            TempObjectPos = ENTITY_COUNT - 100;
    }

    LevelObjects[TempObjectPos].type          = type;
    LevelObjects[TempObjectPos].priority      = 1;
    LevelObjects[TempObjectPos].propertyValue = propertyValue;

    LevelObjects[TempObjectPos].position.x = x;
    LevelObjects[TempObjectPos].position.y = y;
    LevelObjects[TempObjectPos].position.z = z;

    LevelObjects[TempObjectPos].valueA = 0;
    LevelObjects[TempObjectPos].valueB = 0;
    LevelObjects[TempObjectPos].valueC = 0;
    LevelObjects[TempObjectPos].valueD = 0;
    LevelObjects[TempObjectPos].valueE = 0;
    LevelObjects[TempObjectPos].valueF = 0;
}

void DrawObjectModel(int object, float x, float y, float z, float ry, float rx, float rz)
{
    LVertex vertices[4];
    MEM_ZERO(&vertices, sizeof(vertices));

    ushort indices[6] = { 0, 1, 2, 1, 3, 2 };

    memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

    MatrixWorldRotateY(ry);
    MatrixMultiply(&MatrixObject, &MatrixWorld);

    MatrixWorldRotateZ(rx);
    MatrixMultiply(&MatrixObject, &MatrixWorld);

    MatrixWorldRotateX(rz);
    MatrixMultiply(&MatrixObject, &MatrixWorld);

    MatrixWorldTranslateXYZ(x, y, z);
    MatrixMultiply(&MatrixObject, &MatrixWorld);

    switch (object) {
        case OBJ_MODEL_RING: {
            TMF *model = &ObjectModel[object];

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
            SetRenderTexture(0, ObjectTexture[object]);
            SetRenderTextureStageState(0, TEXTURE_STATE_TEXCOORDINDEX, TEXTURE_VALUE_CAMERASPACE_REFLECTIONVECTOR);
            DrawFace(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);
            SetRenderTextureStageState(0, TEXTURE_STATE_TEXCOORDINDEX, TEXTURE_VALUE_PASSTHRU);
            break;
        }

        case OBJ_MODEL_RINGSPARKLE: {
            vertices[0] = { { -2.0f, +2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.05f };
            vertices[1] = { { +2.0f, +2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.05f };
            vertices[2] = { { -2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.05f, 0.99f };
            vertices[3] = { { +2.0f, -2.0f, 0.0f }, 0xFFFFFFFF, 0, 0.99f, 0.99f };

            memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

            MatrixWorldRotateX(rz);
            MatrixMultiply(&MatrixObject, &MatrixWorld);
            MatrixMultiply(&MatrixObject, &MatrixInversed);

            MatrixWorldTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
            SetRenderTexture(0, SparkleTexture);
            SetRenderState(RENDER_STATE_LIGHTING, 0);
            DrawFace(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
            SetRenderState(RENDER_STATE_LIGHTING, 1);
            break;
        }

        default: {
            TMF *model = &ObjectModel[object];

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
            SetRenderTexture(0, ObjectTexture[object]);
            DrawFace(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);
            break;
        }
    }
}

void DrawModelShadow(float x, float y, float z, float magnitude, float sx, float sy, float ry)
{
    LMF *level = &LevelModel;

    Vector3D origin    = { x, y + 1.0f, z };
    Vector3D direction = { 0.0f, 1.0f, 0.0f };

    float point[3];
    float colist[9];

    float c = (x - level->x) / 50.0f;
    float r = (z - level->z) / 50.0f;

    Collided = 0;

    LVertex vertices[4];
    MEM_ZERO(vertices, sizeof(vertices));

    ushort indices[] = { 0, 1, 2, 1, 3, 2 };

    float alpha = 0.5f;
    for (int row = CLAMP(r - 2, 0, level->rows); row < CLAMP(r + 2, 0, level->rows); ++row) {
        for (int col = CLAMP(c - 2, 0, level->columns); col < CLAMP(c + 2, 0, level->columns); ++col) {
            if (LCollision[row][col]->rayCollision(&origin[0], &direction[0], false, 0.0f, magnitude)) {
                LCollision[row][col]->getCollisionPoint(point);
                x = point[0];
                y = point[1] + sy * 0.01f;
                z = point[2];

                LCollision[row][col]->getCollidingTriangles(colist, NULL);
                Collided = 1;
            }
        }
    }

    if (Collided == 1) {
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
        Color color     = PACK_ARGB_F(alpha, 255, 255, 255);

        vertices[0] = { { -sx, 0.0f, +sy }, color, 0, 0.05f, 0.05f };
        vertices[1] = { { +sx, 0.0f, +sy }, color, 0, 0.99f, 0.05f };
        vertices[2] = { { -sx, 0.0f, -sy }, color, 0, 0.05f, 0.99f };
        vertices[3] = { { +sx, 0.0f, -sy }, color, 0, 0.99f, 0.99f };

        memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

        MatrixWorldRotateY(ry);
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        MatrixWorldRotateZ(ASin(normal.z));
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        MatrixWorldRotateX(-ASin(normal.x));
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        MatrixWorldTranslateXYZ(x, y, z);
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
        SetRenderTexture(0, ShadowTexture);
        DrawFace(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
    }
}