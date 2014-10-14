#ifndef __BUS_H__
#define __BUS_H__

#include "Blocker.h"
#include "Cube.h"
#include "Sphere.h"

class Bus : public Blocker {

	Cube *body;
	Sphere *wheels;


protected:
	void init();
public:
	Bus(float* position, Game *game, float velocity, float *direction);
	~Bus();
	void draw(GLuint programID);
	void update();
};
#endif