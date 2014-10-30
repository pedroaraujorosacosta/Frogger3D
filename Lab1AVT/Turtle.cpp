#include "Turtle.h"
#include "Stack.h"
#include "Game.h"
#include <iostream>

Turtle::Turtle(float *position, Game *game, float velocity, float *direction) : MobileObj(position, game, velocity, direction, 2.4f, 2.0f)
{
	init();
	createBufferObjects();
}

Turtle::~Turtle()
{
	delete body;
	delete head;
	delete shell;
}

void Turtle::update(float dt){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity * dt;
}

void Turtle::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the car on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(2.0f, 2.0f, 2.0f);

	if (direction[0] != 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);

	head->draw();

	modelview->push();
	modelview->translateMatrix(0.0, 0.6, 0.1);
	shell->draw();
	modelview->pop();

	modelview->push();

	modelview->translateMatrix(0.0, 0.6, 0.0);
	modelview->rotateMatrix(1.0, 0.0, 0.0, 90);
	body->draw();
	modelview->pop();


	sendDataToShader();
	modelview->pop();
}

void Turtle::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	head = new Sphere(o, game, 0.2 , 8);
	shell = new Sphere(o, game, 0.2, 8);
	body = new Torus(o, game, 0.1, 0.4, 10, 10);

	// set materials
	float ambHead[] = { 0.1f, 0.1f, 0.2f, 1.0f };
	float diffHead[] = { 0.8f, 0.5f, 0.1f, 1.0f };
	float specHead[] = { 0.1f, 0.1f, 0.8f, 1.0f };
	float emissiveHead[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessHead = 100.0f;
	int texcountHead = 0;

	head->setAmbient(ambHead);
	head->setDiffuse(diffHead);
	head->setSpecular(specHead);
	head->setEmissive(emissiveHead);
	head->setShininess(shininessHead);
	head->setTexCount(texcountHead);

	float ambShell[] = { 0.1f, 0.1f, 0.2f, 1.0f };
	float diffShell[] = { 0.8f, 0.5f, 0.8f, 1.0f };
	float specShell[] = { 0.1f, 0.1f, 0.8f, 1.0f };
	float emissiveShell[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessShell = 100.0f;
	int texcountShell = 0;

	shell->setAmbient(ambShell);
	shell->setDiffuse(diffShell);
	shell->setSpecular(specShell);
	shell->setEmissive(emissiveShell);
	shell->setShininess(shininessShell);
	shell->setTexCount(texcountShell);

	float ambBody[] = { 0.1f, 0.1f, 0.2f, 1.0f };
	float diffBody[] = { 0.8f, 0.5f, 0.8f, 1.0f };
	float specBody[] = { 0.1f, 0.1f, 0.8f, 1.0f };
	float emissiveBody[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBody = 100.0f;
	int texcountBody = 0;

	body->setAmbient(ambShell);
	body->setDiffuse(diffShell);
	body->setSpecular(specShell);
	body->setEmissive(emissiveShell);
	body->setShininess(shininessShell);
	body->setTexCount(texcountShell);
}
