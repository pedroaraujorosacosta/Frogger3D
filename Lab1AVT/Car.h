#ifndef __CAR_H__
#define __CAR_H__

#include "Blocker.h"

class Torus;
class Sphere;
class Cylinder;

class Car : public MobileObj {

	Cylinder *body;
	Torus *wheels;
	Sphere *cabin;

	const float carW = 1.2f;
	const float carH = 0.8f;

protected:
	void init();
public:
	Car(float* position, Game *game, float velocity, float *direction);
	~Car();
	void draw();
	void update(float dt);
};
#endif