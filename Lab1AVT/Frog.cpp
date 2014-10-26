#include "Frog.h"
#include "Stack.h"
#include "Game.h"
#include <iostream>

Frog::Frog(float *position, Game *game, float velocity, float *direction) : Blocker(position, game, velocity, direction)
{
	init();
	createBufferObjects();
}

Frog::~Frog()
{
	delete body;
	delete eyes;
}

void Frog::update(float dt)
{
	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity * dt;
}

void Frog::move(float *direction)
{
	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];
	velocity = 1;
}

void Frog::move()
{
	velocity = 1;
}

void Frog::stop() 
{
	velocity = 0.0;
}

void Frog::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the frog on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(0.9, 0.8, 0.6);  
	
	if (direction[0] < 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, -90);
	else if (direction[0] > 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);
	else if (direction[1] < 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 180);

	body->draw();
	
	modelview->push();
	modelview->translateMatrix(0.3, 0.6, 0.0);
	eyes->draw();
	modelview->pop();
	
	modelview->push();
	modelview->translateMatrix(-0.3, 0.6, 0.0);
	eyes->draw();
	modelview->pop();

	sendDataToShader();
	modelview->pop();
}

void Frog::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Sphere(o, game, 0.6, 8);
	eyes = new Sphere(o, game, 0.2, 8);

	// set materials
	float ambBody[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diffBody[] = { 0.1f, 0.6f, 0.1f, 1.0f };
	float specBody[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissiveBody[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBody = 100.0f;
	int texcountBody = 0;

	body->setAmbient(ambBody);
	body->setDiffuse(diffBody);
	body->setSpecular(specBody);
	body->setEmissive(emissiveBody);
	body->setShininess(shininessBody);
	body->setTexCount(texcountBody);

	float ambEyes[] = { 0.1f, 0.2f, 0.1f, 1.0f };
	float diffEyes[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specEyes[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissiveEyes[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessEyes = 100.0f;
	int texcountEyes = 0;

	eyes->setAmbient(ambEyes);
	eyes->setDiffuse(diffEyes);
	eyes->setSpecular(specEyes);
	eyes->setEmissive(emissiveEyes);
	eyes->setShininess(shininessEyes);
	eyes->setTexCount(texcountEyes);
}

float Frog::getX()
{
	return position[0];
}

float Frog::getY()
{
	return position[1];
}

float Frog::getZ()
{
	return position[2];
}