#ifndef __BUS_H__
#define __BUS_H__

#include "Blocker.h"
#include "Cube.h"
#include "Sphere.h"

class Torus;

class Bus : public Blocker {

	Cube *body;
	Torus *wheels;


protected:
	void init();
public:
	Bus(float* position, Game *game, float velocity, float *direction);
	~Bus();
	void draw();
	void update(float dt);
};
#endif