#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Object.h"

class Sphere : public Object {
protected:
	void init();
public:
	Sphere(float* position, Game *game, float radius, int divisions);
	void draw();
};
#endif