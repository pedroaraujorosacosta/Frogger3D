#ifndef __CAR_H__
#define __CAR_H__

#include "Blocker.h"

class Torus;
class Sphere;
class Cylinder;

class Car : public Blocker {

	Cylinder *body;
	Torus *wheels;
	Sphere *cabin;

protected:
	void init();
public:
	Car(float* position, Game *game, float velocity, float *direction);
	~Car();
	void draw();
	void update(float dt);
};
#endif