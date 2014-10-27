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
	void draw(GLuint programID);
	void update();
	void move(float *direction);
	void move();
	void stop();
	float getX();
	float getY();
	float getZ();

	int getLife();
	void setLife(int _life);
};
#endif