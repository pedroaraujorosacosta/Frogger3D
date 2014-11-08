#ifndef __QUAD_H__
#define __QUAD_H__

#include "Object.h"

class Quad : public Object {
protected:
	void init();
public:
	Quad(float* position, Game *game);
	void draw();
};
#endif