#ifndef __TORUS_H__
#define __TORUS_H__

#include "Object.h"

class Torus : public Object {
protected:
	void init();
public:
	Torus(float* position, Game *game, float innerRadius, float outerRadius, int rings, int sides);
	void draw();
};
#endif