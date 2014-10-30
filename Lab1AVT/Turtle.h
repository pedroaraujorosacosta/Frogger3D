#ifndef __TURTLE_H__
#define __TURTLE_H__

#include "Surface.h"
#include "Torus.h"
#include "Sphere.h"

class Turtle : public MobileObj {

	Sphere *head;
	Torus *body;
	Sphere *shell;

	const float turtleW = 2.0f;
	const float turtleH = 2.0f;

protected:
	void init();
public:
	Turtle(float* position, Game *game, float velocity, float *direction);
	~Turtle();
	void draw();
	void update(float dt);
};
#endif