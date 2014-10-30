#ifndef __BUS_H__
#define __BUS_H__

#include "Blocker.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"

class Torus;

class Bus : public MobileObj {

	Cube *body;
	Torus *wheels;
	Cylinder *bumper;

	const float busW = 2.4f;
	const float busH = 1.0f;

protected:
	void init();
public:
	Bus(float* position, Game *game, float velocity, float *direction);
	~Bus();
	void draw();
	void update(float dt);
};
#endif