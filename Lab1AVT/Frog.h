#ifndef __FROG_H__
#define __FROG_H__

#include "Cube.h"
#include "Sphere.h"
#include "MobileObj.h"

class Frog : public MobileObj {

	Sphere *body;
	Sphere *eyes;

	int life;

	const float FROG_VELOCITY = 0.3f;
	const float FROG_W = 1.4f;
	const float FROG_H = 1.0f;
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
	void setPosition(float* pos);
	void setDirection(float* dir);
	void setVelocity(float v);

	int getLife();
	void setLife(int _life);
};
#endif