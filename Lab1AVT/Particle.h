#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "BillboardObj.h"

class Game;
class Quad;

class Particle : public BillboardObj {


	float v[3];
	float a[3];
	float rgba[4];
	float life;
	float fade;
	const float MAXLIFE = 1.0;
	GLuint texname;
	Game* game;

public:
	Particle(float* pos, float *faceDirection, Game* game, int width, int height);
	void reset();
	void update(float dt);
	bool isAlive();
	void respawn(float posX, float posY, float posZ);
	void init();
	void draw();
};

#endif