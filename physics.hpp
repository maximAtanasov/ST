#ifndef PHYSICS_DEF
#define PHYSICS_DEF

#include "defs.hpp"

void setGravity(float arg);
int checkCollision(unsigned int ID);
void applyPhysics();


extern int gravity;
extern int friction;
extern int Levelfloor;

#endif
