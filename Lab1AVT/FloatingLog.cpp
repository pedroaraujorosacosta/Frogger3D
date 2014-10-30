#include "FloatingLog.h"
#include "Stack.h"
#include "Game.h"
#include <iostream>

FloatingLog::FloatingLog(float *position, Game *game, float velocity, float *direction) : MobileObj(position, game, velocity, direction, 4.0f, 1.0f)
{
	init();
	createBufferObjects();
}

FloatingLog::~FloatingLog()
{
	delete body;

	delete front;
	delete back;
}

void FloatingLog::update(float dt){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity;
}

void FloatingLog::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the car on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(2.0, 1.0, 1.0);

	if (direction[0] != 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);

	body->draw();

		modelview->push();

		modelview->translateMatrix(0.0,-0.7,0.0);
		front->draw();
		modelview->pop();

		modelview->push();

		modelview->translateMatrix(0.0,0.7,0.0);
		back->draw();
		modelview->pop();

	sendDataToShader();
	modelview->pop();
}

void FloatingLog::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Cylinder(o, game, 2.0 ,0.5, 10);


	front = new Cylinder(o, game, 0.5, 0.6, 10);
	back = new Cylinder(o, game, 0.5, 0.6, 10);

	// set materials
	float ambBody[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffBody[] = { 0.7f, 0.3f, 0.1f, 1.0f };
	float specBody[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveBody[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBody = 100.0f;
	int texcountBody = 0;

	body->setAmbient(ambBody);
	body->setDiffuse(diffBody);
	body->setSpecular(specBody);
	body->setEmissive(emissiveBody);
	body->setShininess(shininessBody);
	body->setTexCount(texcountBody);


	// set materials
	float ambFront[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffFront[] = { 0.2f, 0.7f, 0.1f, 1.0f };
	float specFront[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveFront[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessFront = 100.0f;
	int texcountFront = 0;

	front->setAmbient(ambFront);
	front->setDiffuse(diffFront);
	front->setSpecular(specFront);
	front->setEmissive(emissiveFront);
	front->setShininess(shininessFront);
	front->setTexCount(texcountFront);


	// set materials
	float ambBack[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffBack[] = { 0.1f, 0.3f, 0.3f, 1.0f };
	float specBack[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveBack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBack = 100.0f;
	int texcountBack = 0;

	back->setAmbient(ambBack);
	back->setDiffuse(diffBack);
	back->setSpecular(specBack);
	back->setEmissive(emissiveBack);
	back->setShininess(shininessBack);
	back->setTexCount(texcountBack);
}
