#ifndef __CAR_H__
#define __CAR_H__

#include "Blocker.h"
#include "Cube.h"
#include "Torus.h"

class Car : public Blocker {

	Cube *body;
	Torus *wheels;


protected:
	void init();
public:
	Car(float* position, Game *game, float velocity, float *direction);
	~Car();
	void draw();
	void update(float dt);
};
#endif