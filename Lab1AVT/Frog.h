#ifndef __FROG_H__
#define __FROG_H__

#include "Blocker.h"
#include "Cube.h"
#include "Sphere.h"

class Frog : public Blocker {

	Sphere *body;
	Sphere *eyes;

	int life;


protected:
	void init();
public:
	Frog(float* position, Game *game, float velocity, float *direction, int life);
	~Frog();
	void draw();
	void update(float dt);
	void move(float *direction);
	void move();
	void stop();
	void killed();

	int getLife();
	void setLife(int _life);
};
#endif