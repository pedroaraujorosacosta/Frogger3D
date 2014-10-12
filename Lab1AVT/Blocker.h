#ifndef __BLOCKER_H__
#define __BLOCKER_H__

#include "MobileObj.h"

class Blocker: public MobileObj {

	
protected:
	virtual void init();
public:
	Blocker(float* position, Game *game, float velocity, float *direction);
	virtual void draw(GLuint programID);
};
#endif