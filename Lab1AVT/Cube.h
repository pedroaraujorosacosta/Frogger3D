#ifndef __CUBE_H__
#define __CUBE_H__

#include "Object.h"

class Cube : public Object {
protected:
	void init();
public:
	Cube(float* position, Game *game);
	void draw();
};
#endif