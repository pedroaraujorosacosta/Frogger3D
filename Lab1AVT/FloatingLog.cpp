#include "FloatingLog.h"
#include "Stack.h"
#include "Game.h"
#include <iostream>

FloatingLog::FloatingLog(float *position, Game *game, float velocity, float *direction) : Surface(position, game, velocity, direction)
{
	init();
	createBufferObjects();
}

FloatingLog::~FloatingLog()
{
	delete body;
}

void FloatingLog::update(){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity;
}

void FloatingLog::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the car on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(2.0, 1.0, 1.0);

	if (direction[0] != 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);

	body->draw(programID);

	sendDataToShader(programID);
	modelview->pop();
}

void FloatingLog::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Cylinder(o, game, 2.0 ,0.5, 10);

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
}
