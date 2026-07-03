#ifndef COLLISION_H
#define COLLISION_H

extern byte Collided;
extern CollisionModel3D ***LCollision;

byte ObjectFloorCollision(float *xpos, float *ypos, float *zpos, float xdir, float ydir, float zdir);

inline byte ObjectFloorCollision(Vector3D *position, Vector3D direction)
{
    return ObjectFloorCollision(&position->x, &position->y, &position->z, direction.x, direction.y, direction.z);
}

#endif // !COLLISION_H