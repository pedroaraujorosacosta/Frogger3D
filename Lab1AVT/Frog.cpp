#include "Frog.h"
#include "Stack.h"
#include "Game.h"
#include <iostream>
#include "Light.h"
#include "Vector.h"

Frog::Frog(float *position, Game *game, float velocity, float *direction, int life) : Blocker(position, game, velocity, direction)
{
	init();
	createBufferObjects();
	this->life = life;
}

Frog::~Frog()
{
	delete body;
	delete eyes;
}

void Frog::update(float dt)
{
	if (velocity > 0.0f)
	{
		for (int i = 0; i < 3; i++)
			position[i] += direction[i] * velocity;

		// adjust the miner's spot light
		Light *spot = game->getSpotLight();
		
		float newSpotDirf[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++) newSpotDirf[i] = direction[i];
		Vector newSpotDir(newSpotDirf, 4);
		newSpotDir.normalize();

		float hNewSpotPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		for (int i = 0; i < 3; i++) hNewSpotPos[i] = position[i] + 0.5*direction[i];
		Vector newSpotPos(hNewSpotPos, 4);
		spot->setPosition(newSpotPos);

		// Check bounds
		if (position[1] > -5){
			position[1] = -13;
			setLife(getLife() - 1);
		}
	}
}

void Frog::move(float *direction)
{
	Light *spot = game->getSpotLight();
	float newSpotDirf[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 3; i++) newSpotDirf[i] = direction[i];
	Vector newSpotDir(newSpotDirf, 4);
	newSpotDir.normalize();
	spot->setDirection(newSpotDir);

	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];
	velocity = 0.08;
}

void Frog::move()
{
	velocity = 0.08;
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
	modelview->scaleMatrix(1.2, 1.0, 0.8);  
	
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

	Light *spot = game->getSpotLight();
	if (spot != 0) 
	{
		float newSpotDirf[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++) newSpotDirf[i] = direction[i];
		Vector newSpotDir(newSpotDirf, 4);
		newSpotDir.normalize();
		spot->setDirection(newSpotDir);

		float hNewSpotPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		for (int i = 0; i < 3; i++) hNewSpotPos[i] = position[i] + 0.5*direction[i];
		Vector newSpotPos(hNewSpotPos, 4);
		spot->setPosition(newSpotPos);
	}
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

int Frog::getLife()
{
	return life;
}

void Frog::setLife(int life)
{
	this->life = life;
}