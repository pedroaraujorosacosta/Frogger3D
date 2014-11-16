#include <stdlib.h>
#include <math.h>
#include "Particle.h"
#include "glbmp.h" 
#include "Stack.h"
#include "Game.h"

#define frand()	((float) rand() /RAND_MAX)
#ifndef M_PI
#define M_PI 3.14159265
#endif

Particle::Particle(float* pos , Game* game, int width, int height) 
: BillboardObj(pos, game, pos, width, height) //corrigir dire��o
{
	this->game = game;
	/*glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);*/
	LoadTexture("particula.bmp");
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
	a[1] = -0.15f; /* simular a acelera��o da gravidade */
	a[2] = 0.0f;

	/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
	rgba[0] = 0.882f;
	rgba[1] = 0.552f;
	rgba[2] = 0.211f;
	rgba[3] = 1.0f;

	life = MAXLIFE;		/* vida inicial */
	fade = 0.005f;	    /* step de decr�scimo da vida para cada itera��o */
}

void Particle::draw() {
	int i;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texname);
	if (isAlive()) {

		/* A vida da part�cula representa o canal alpha da cor. Como o blend est� activo a cor final � a soma da cor rgb do fragmento multiplicada pelo
		alpha com a cor do pixel destino */
		float pos[3] = { 0.0, 0.0, 0.0 };
		Quad* quad = new Quad(pos, game);
		quad->setAmbient(rgba);
		quad->setDiffuse(rgba);
		quad->setSpecular(rgba);

		Stack* modelview = game->getModelViewStack();

		modelview->push();
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

void Particle::LoadTexture(const char * bitmap_file)
{

	glbmp_t bitmap;     //object to fill with data from glbmp

	//try to load the specified file--if it fails, dip out
	if (!glbmp_LoadBitmap(bitmap_file, 0, &bitmap))
	{
		fprintf(stderr, "Error loading bitmap file: %s\n", bitmap_file);
		exit(1);
	}

	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);
	//copy data from bitmap into texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height,
		0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); /* n�o era necess�rio pois este comando � executado por omiss�o */
	//free the bitmap
	glbmp_FreeBitmap(&bitmap);
}