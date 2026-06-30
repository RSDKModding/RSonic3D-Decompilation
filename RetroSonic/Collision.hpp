#ifndef COLLISION_H
#define COLLISION_H

extern sbyte Collided;
extern CollisionModel3D ***LCollision;

sbyte ObjectFloorCollision(float *xpos, float *ypos, float *zpos, float xdir, float ydir, float zdir);

#endif // !COLLISION_H