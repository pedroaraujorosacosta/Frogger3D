#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "BillboardObj.h"

class Particle : public BillboardObj {

	float pos[3];
	float v[3];
	float a[3];
	float size;
	float rgba[4];
	float life;
	float decay;
	const float MAXLIFE;

public:
	Particle(float* pos, float *v, float* a, float size, float* rgba, float life, float decay, Game* game, int width, int height);
	void reset();
	void update(float dt);
	bool isAlive();
	void respawn(float posX, float posY, float posZ);

};

#endif