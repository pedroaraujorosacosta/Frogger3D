#ifndef __MOBILEOBJ_H__
#define __MOBILEOBJ_H__

#include "Object.h"

class MobileObj : public Object {

protected:
	float direction[3];
	float velocity;
	float initVelocity;
	float width;
	float height;

	virtual void init();
public:
	MobileObj(float* position, Game *game, float velocity, float *direction, float width, float height);
	virtual void draw();
	float getDirX();
	float getDirY();
	float getDirZ();
	float getWidth();
	void multiplyVelocity(float dificulty);
	void reset();
};
#endif