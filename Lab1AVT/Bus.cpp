#include "Bus.h"
#include "Stack.h"
#include "Game.h"
#include "Torus.h"
#include <iostream>

Bus::Bus(float *position, Game *game, float velocity, float *direction) : Blocker(position, game, velocity, direction)
{
	init();
	createBufferObjects();
}

Bus::~Bus()
{
	delete body;
	delete wheels;
	delete bumper;
}

void Bus::update(float dt){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity * dt;
}

void Bus::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the car on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(1.3, 1.0, 1.0);

	if (direction[0] != 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);

	modelview->push();
	modelview->scaleMatrix(1.0, 2.0, 1.0);
	body->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.5, 0.6, -0.5);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.5, -0.6, -0.5);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(-0.5, -0.6, -0.5);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(-0.5, 0.6, -0.5);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	wheels->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.0f, -0.75f, -0.075f);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	modelview->scaleMatrix(1.0f, 1.0f, 1.20f);
	bumper->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.0f, 0.5f, -0.35f);
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
	modelview->scaleMatrix(1.5f, 1.0f, 1.20f);
	bumper->draw();
	modelview->pop();

	sendDataToShader();
	modelview->pop();
}

void Bus::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Cube(o, game);
	wheels = new Torus(o, game, 0.1f, 0.3f, 8, 8);
	bumper = new Cylinder(o, game, 1.0f, 0.5f, 15);

	// set materials
	float ambBody[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffBody[] = { 0.9f, 0.8f, 0.8f, 1.0f };
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

	bumper->setAmbient(ambBody);
	bumper->setDiffuse(diffBody);
	bumper->setSpecular(specBody);
	bumper->setEmissive(emissiveBody);
	bumper->setShininess(shininessBody);
	bumper->setTexCount(texcountBody);

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
