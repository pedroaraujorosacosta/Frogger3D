#include "Car.h"
#include "Stack.h"
#include "Game.h"
#include "Torus.h"
#include "Sphere.h"
#include "Cylinder.h"
#include <iostream>

Car::Car(float *position, Game *game, float velocity, float *direction) : MobileObj(position, game, velocity, direction, 1.2f, 0.8f)
{
	init();
	createBufferObjects();
}

Car::~Car()
{
	delete body;
	delete wheels;
	delete cabin;
}

void Car::update(float dt){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity * dt;
}

void Car::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the car on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(carW, carH, 0.5f);
	
	if (direction[0] != 0.0)
		modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);

	// body draw
	modelview->push();
	modelview->translateMatrix(0.0f, 0.0f, -0.35f);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	modelview->scaleMatrix(1.5f, 1.0f, 0.7f);
	body->draw();
	modelview->pop();

	// cabin draw
	modelview->push();
	//modelview->translateMatrix(0.0f, 0.0f, -0.35f);
	modelview->scaleMatrix(1.0f, 1.0f, 1.0f);
	cabin->draw();
	modelview->pop();

	// Wheels draw
	modelview->push();
	modelview->translateMatrix(0.5f, 0.3f, -0.5f);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.5, -0.3, -0.5);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();
	
	modelview->push();
	modelview->translateMatrix(-0.5, -0.3,-0.5);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(-0.5, 0.3, -0.5);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();

	sendDataToShader();
	modelview->pop();
}

void Car::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Cylinder(o, game, 1.0f, 0.5f, 15);
	wheels = new Torus(o, game, 0.1f, 0.3f, 8, 8);
	cabin = new Sphere(o, game, 0.5f, 8);

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

	float ambCabin[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffCabin[] = { 0.2f, 0.2f, 0.8f, 1.0f };
	float specCabin[] = { 0.2f, 0.2f, 0.9f, 1.0f };
	float emissiveCabin[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessCabin = 100.0f;
	int texcountCabin = 0;

	cabin->setAmbient(ambCabin);
	cabin->setDiffuse(diffCabin);
	cabin->setSpecular(specCabin);
	cabin->setEmissive(emissiveCabin);
	cabin->setShininess(shininessCabin);
	cabin->setTexCount(texcountCabin);
}
