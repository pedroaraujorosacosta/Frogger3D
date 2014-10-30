#ifndef __FLOATINGLOG_H__
#define __FLOATINGLOG_H__

#include "Surface.h"
#include "Cylinder.h"

class FloatingLog : public MobileObj {

	Cylinder *body;

	Cylinder *front;
	Cylinder *back;

	const float logW = 4.0f;
	const float logH = 1.0f;

protected:
	void init();
public:
	FloatingLog(float* position, Game *game, float velocity, float *direction);
	~FloatingLog();
	void draw();
	void update(float dt);
};
#endif