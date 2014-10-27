#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "Object.h"

class Cylinder : public Object {
protected:
	void init();
public:
	Cylinder(float* position, Game *game, float height, float radius, int sides);
	void draw();
};
#endif