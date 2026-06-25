#ifndef COLLISION_H
#define COLLISION_H

extern char byte_420508;
extern CollisionModel3D **LCollision;

sbyte ObjectFloorCollision(float *xpos, float *ypos, float *zpos, float xvel, float yvel, float zvel);

#endif // !COLLISION_H