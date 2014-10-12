#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "MobileObj.h"

class Surface : public MobileObj {


protected:
	virtual void init();
public:
	Surface(float* position, Game *game, float velocity, float *direction);
	virtual void draw(GLuint programID);
};
#endif