#include <stdlib.h>
#include <math.h>
#include "Particle.h"

#define frand()	((float) rand() /RAND_MAX)
#ifndef M_PI
#define M_PI 3.14159265
#endif

Particle::Particle(float* pos, float *v, float* a, float size, float* rgba, float life, float decay, Game* game, int width, int height) 
: BillboardObj(position, game, v, width, height), MAXLIFE(life)
{
	for (int i = 0; i < 3; i++) {
		this->pos[i] = pos[i];
		v[i] = v[i];
		this->a[i] = a[i];
	}
	for (int i = 0; i < 4; i++) 
		this->rgba[i] = rgba[i];
	this->life = life;
	this->decay = decay;
	this->size = size;
	respawn(pos[0], pos[1], pos[2]);
}


void Particle::reset(){
	this->life = MAXLIFE;
}

void Particle::update(float dt)
{
	float delta;
	delta = dt * 0.00378;
	pos[0] += v[0] * delta;
	pos[1] += v[1] * delta;
	pos[2] += v[2] * delta;
	v[0] += a[0] * delta;
	v[1] += a[1] * delta;
	v[2] += a[2] * delta;
	life -= decay;
}

void Particle::respawn(float posX, float posY, float posZ)
{
	double v = 0.8f * frand() + 0.2f;
	double phi = frand()*M_PI;
	double theta = 2.0f*frand()*M_PI;
	pos[0] = posX;
	pos[1] = posY;
	pos[2] = posZ;
	this->v[0] = v * cos(theta) * sin(phi);
	this->v[1] = v * sin(theta) * sin(phi);
	this->v[2] = v * cos(phi);
	reset();
}

bool Particle::isAlive()
{
	return life > 0.0f;
}