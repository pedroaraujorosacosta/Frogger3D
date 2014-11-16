#include <stdlib.h>
#include <math.h>
#include "Particle.h"
#include "Stack.h"
#include "Game.h"

#define frand()	((float) rand() /RAND_MAX)
#ifndef M_PI
#define M_PI 3.14159265
#endif

Particle::Particle(float* pos , Game* game, int width, int height) 
: BillboardObj(pos, game, pos, width, height) //corrigir direção
{
	this->game = game;
	/*glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);*/
	init();
}

void Particle::init()
{
	GLfloat v, theta, phi;
	int i;

	v = 0.8*frand() + 0.2;
	phi = frand()*M_PI;
	theta = 2.0*frand()*M_PI;
	pos[0] = 0.0f;
	pos[1] = 15.0f;
	pos[2] = 0.0f;
	this->v[0] = v * cos(theta) * sin(phi);
	this->v[1] = v * cos(phi);
	this->v[2] = v * sin(theta) * sin(phi);
	a[0] = 0.1f; /* simular um pouco de vento */
	a[1] = -0.15f; /* simular a aceleração da gravidade */
	a[2] = 0.0f;

	/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
	rgba[0] = 0.882f;
	rgba[1] = 0.552f;
	rgba[2] = 0.211f;
	rgba[3] = 1.0f;

	life = MAXLIFE;		/* vida inicial */
	fade = 0.005f;	    /* step de decréscimo da vida para cada iteração */

	float pos[3] = { 0.0, 0.0, 0.0 };
	quad = new Quad(pos, game);
	quad->setAmbient(rgba);
	quad->setDiffuse(rgba);
	quad->setSpecular(rgba);
	quad->setTexCount(5);
}

void Particle::draw() {
	if (isAlive()) {
		Stack* modelview = game->getModelViewStack();

		modelview->push();
		modelview->translateMatrix(this->pos[0], this->pos[1], this->pos[2]);
		quad->draw();
		modelview->pop();
	}

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
	life -= fade;
	rgba[3] = life;
	std::cout << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
	//glutPostRedisplay();
	//glutTimerFunc(33, update, 1);
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

