#ifndef __FLOATINGLOG_H__
#define __FLOATINGLOG_H__

#include "Surface.h"
#include "Cube.h"
#include "Sphere.h"

class FloatingLog : public Surface {

	Cube *body;
	Sphere *wheels;


protected:
	void init();
public:
	FloatingLog(float* position, Game *game, float velocity, float *direction);
	~FloatingLog();
	void draw(GLuint programID);
	void update();
};
#endif