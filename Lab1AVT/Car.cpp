#include "Car.h"
#include "Stack.h"
#include "Game.h"
#include <iostream>

Car::Car(float *position, Game *game, float velocity, float *direction) : Blocker(position, game, velocity, direction)
{
	init();
	createBufferObjects();
}

Car::~Car()
{
	delete body;
	delete wheels;
}

void Car::update(){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity;
}

void Car::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the car on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(1.2, 0.8, 1.0);
	
	if (direction[0] != 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);

	body->draw(programID);

	modelview->push();
	modelview->translateMatrix(0.5,0.3,-0.5);
	wheels->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.5, -0.3, -0.5);
	wheels->draw(programID);
	modelview->pop();
	
	modelview->push();
	modelview->translateMatrix(-0.5, -0.3,-0.5);
	wheels->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(-0.5, 0.3, -0.5);
	wheels->draw(programID);
	modelview->pop();

	sendDataToShader(programID);
	modelview->pop();
}

void Car::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Cube(o, game);
	wheels = new Sphere(o, game, 0.2, 8);

	// set materials
	float ambBody[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffBody[] = { 0.6f, 0.2f, 0.9f, 1.0f };
	float specBody[] = { 0.6f, 0.2f, 0.9f, 1.0f };
	float emissiveBody[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBody = 100.0f;
	int texcountBody = 0;

	body->setAmbient(ambBody);
	body->setDiffuse(diffBody);
	body->setSpecular(specBody);
	body->setEmissive(emissiveBody);
	body->setShininess(shininessBody);
	body->setTexCount(texcountBody);

	float ambWheels[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffWheels[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float specWheels[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveWheels[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessWheels = 100.0f;
	int texcountWheels = 0;

	wheels->setAmbient(ambWheels);
	wheels->setDiffuse(diffWheels);
	wheels->setSpecular(specWheels);
	wheels->setEmissive(emissiveWheels);
	wheels->setShininess(shininessWheels);
	wheels->setTexCount(texcountWheels);
}
