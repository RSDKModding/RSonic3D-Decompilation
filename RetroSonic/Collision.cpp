#include "RetroEngine.hpp"

byte Collided;
CollisionModel3D ***LCollision;

byte ObjectFloorCollision(float *xpos, float *ypos, float *zpos, float xdir, float ydir, float zdir)
{
    Vector3D origin    = { *xpos, *ypos, *zpos };
    Vector3D direction = { xdir, ydir, zdir };
    float magnitude    = direction.Magnitude();

    direction = direction.Normalized();

    float point[3];
    float colist[9];

    float c = (origin.x - LevelModel.startX) / 50.0f;
    float r = (origin.z - LevelModel.startZ) / 50.0f;

    int minRow = CLAMP(r - 4, 0, LevelModel.rows);
    int maxRow = CLAMP(r + 4, 0, LevelModel.rows);

    int minCol = CLAMP(c - 4, 0, LevelModel.columns);
    int maxCol = CLAMP(c + 4, 0, LevelModel.columns);

    Collided = false;

    for (int y = minRow; y < maxRow; ++y) {
        for (int x = minCol; x < maxCol; ++x) {
            if (!LCollision[y][x]->rayCollision(&origin[0], &direction[0], true, 0.0f, magnitude))
                continue;

            Collided = true;

            LCollision[y][x]->getCollisionPoint(point);
            *xpos = point[0];
            *ypos = point[1];
            *zpos = point[2];

            LCollision[y][x]->getCollidingTriangles(colist, NULL);

            Vector3D ab = { colist[3] - colist[0], colist[4] - colist[1], colist[5] - colist[2] };
            Vector3D ac = { colist[6] - colist[0], colist[7] - colist[1], colist[8] - colist[2] };

            Vector3D normal = CrossProduct(ab, ac).Normalized();

            PlayerTargetRotationZ = ASin(normal.z);
            PlayerTargetRotationX = -ASin(normal.x);

            if (normal.y < 0.0f)
                PlayerTargetRotationX = -PlayerTargetRotationX + RSDK_PI;
        }
    }

    if (!Collided) {
        PlayerTargetRotationX = 0.0f;
        PlayerTargetRotationZ = 0.0f;
    }

    return Collided;
}