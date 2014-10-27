#ifndef __FLOATINGLOG_H__
#define __FLOATINGLOG_H__

#include "Surface.h"
#include "Cylinder.h"

class FloatingLog : public Surface {

	Cylinder *body;

	Cylinder *front;
	Cylinder *back;


protected:
	void init();
public:
	FloatingLog(float* position, Game *game, float velocity, float *direction);
	~FloatingLog();
	void draw();
	void update(float dt);
};
#endif