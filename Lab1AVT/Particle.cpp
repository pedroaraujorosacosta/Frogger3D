#include <stdlib.h>
#include <math.h>
#include "Particle.h"
#include "MatrixStack.h"
#include "Game.h"

#define frand()	((float) rand() /RAND_MAX)
#ifndef M_PI
#define M_PI 3.14159265
#endif

Particle::Particle(float* pos, float *faceDirection, Game* game, int width, int height) 
	: BillboardObj(pos, game, faceDirection, width, height) //corrigir direção
{
	this->game = game;
	for (int i = 0; i < 3; i++)
		position[i] = pos[i];

	init();
}

void Particle::init()
{
	GLfloat v, theta, phi;
	int i;

	v = 0.8*frand() + 0.2;
	phi = frand()*M_PI;
	theta = 2.0*frand()*M_PI;
	this->v[0] = v * cos(theta) * sin(phi);
	this->v[1] = v * cos(phi);
	this->v[2] = v * sin(theta) * sin(phi);
	a[0] = 0.1f; /* simular um pouco de vento */
	a[1] = -0.15f; 
	a[2] = -0.8f; /* simular a aceleração da gravidade */

	/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
	rgba[0] = 0.882f;
	rgba[1] = 0.552f;
	rgba[2] = 0.211f;
	rgba[3] = 0.01f;

	life = MAXLIFE;		/* vida inicial */
	fade = 0.005f;	    /* step de decréscimo da vida para cada iteração */

	quad->setAmbient(rgba);
	quad->setDiffuse(rgba);
	quad->setSpecular(rgba);
	quad->setTexCount(5);
	setTexCount(5);
}

void Particle::draw() {
	if (isAlive()) {
		MatrixStack* modelview = game->getModelViewStack();

		modelview->push();
		modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
		modelview->scaleMatrix(0.2f, 0.2f, 0.2f);
		BillboardObj::draw();
		modelview->pop();
	}

}

void Particle::reset(){
	this->life = MAXLIFE;
}

void Particle::update(float dt)
{
	position[0] += v[0] * dt;
	position[1] += v[1] * dt;
	position[2] += v[2] * dt;
	v[0] += a[0] * dt;
	v[1] += a[1] * dt;
	v[2] += a[2] * dt;
	life -= fade;
	rgba[3] = life;
}


void Particle::respawn(float posX, float posY, float posZ)
{
	double v = 0.8f * frand() + 0.2f;
	double phi = frand()*M_PI;
	double theta = 2.0f*frand()*M_PI;
	position[0] = posX;
	position[1] = posY;
	position[2] = posZ;
	this->v[0] = v * cos(theta) * sin(phi);
	this->v[1] = v * sin(theta) * sin(phi);
	this->v[2] = v * cos(phi);
	reset();
}

bool Particle::isAlive()
{
	return life > 0.0f;
}

