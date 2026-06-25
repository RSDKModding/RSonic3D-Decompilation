#include "RetroEngine.hpp"

char byte_420508;
CollisionModel3D **LCollision;

sbyte ObjectFloorCollision(float *xpos, float *ypos, float *zpos, float xvel, float yvel, float zvel)
{
    Vector3D position;
    Vector3D velocity;
    Vector3D point;

    velocity.x = xvel;
    velocity.y = yvel;
    velocity.z = zvel;

    float magnitude = velocity.Magnitude();

    velocity = velocity.Normalized();

    position.x = *xpos;
    position.y = *ypos;
    position.z = *zpos;

    int mnx = (int)((position.x - levelLMF.variable_4) / 50.0f) - 4;
    int mxx = mnx + 8;

    int mny = (int)((position.z - levelLMF.variable_5) / 50.0f) - 4;
    int mxy = mny + 8;

    mnx = CLAMP(mnx, 0, levelLMF.variable_2);
    mxx = CLAMP(mxx, 0, levelLMF.variable_2);

    mny = CLAMP(mny, 0, levelLMF.variable_3);
    mxy = CLAMP(mxy, 0, levelLMF.variable_3);

    byte_420508 = false;

    int modelID = mny * levelLMF.variable_2 + mnx;
    for (int cy = mny; cy < mxy; ++cy) {
        for (int cx = mnx; cx < mxx; ++cx) {
            if (LCollision[modelID]->rayCollision(&position[0], &velocity[0], true, 0.0f, magnitude)) {
                byte_420508 = true;

                LCollision[modelID]->getCollisionPoint(&point[0]);
                *xpos = point.x;
                *ypos = point.y;
                *zpos = point.z;

                float colist[9];
                LCollision[modelID]->getCollidingTriangles(colist, nullptr);

                Vector3D vector1;
                vector1.x = colist[3] - colist[0];
                vector1.y = colist[4] - colist[1];
                vector1.z = colist[5] - colist[2];

                Vector3D vector2;
                vector2.x = colist[6] - colist[0];
                vector2.y = colist[7] - colist[1];
                vector2.z = colist[8] - colist[2];

                Vector3D vector3 = CrossProduct(vector1, vector2).Normalized();

                data_4C9D5C = ASin(vector3.z);
                if (vector3.y >= 0.0f)
                    data_4C9D60 = -ASin(vector3.x);
                else
                    data_4C9D60 = ASin(vector3.x) - 3.1415927 + 3.1415927 + 3.1415927;
            }

            ++modelID;
        }

        modelID += levelLMF.variable_2 - (mxx - mnx);
    }

    if (!byte_420508) {
        data_4C9D60 = 0.0f;
        data_4C9D5C = 0.0f;
    }

    return byte_420508;
}