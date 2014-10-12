#ifndef __CAR_H__
#define __CAR_H__

#include "Blocker.h"
#include "Cube.h"
#include "Sphere.h"

class Car : public Blocker {

	Cube *body;
	Sphere *wheels;


protected:
	void init();
public:
	Car(float* position, Game *game, float velocity, float *direction);
	~Car();
	void draw(GLuint programID);
	void update();
};
#endif