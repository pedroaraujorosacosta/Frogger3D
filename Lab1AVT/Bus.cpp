#include "Bus.h"
#include "Stack.h"
#include "Game.h"
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
}

void Bus::update(){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity;
}

void Bus::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the car on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(2.0, 2.0, 1.0);

	if (direction[0] != 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);

	modelview->push();
	modelview->scaleMatrix(1.0, 2.0, 1.0);
	body->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.6, 0.6, 0.0);
	wheels->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.6, -0.6, 0.0);
	wheels->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(-0.6, -0.6, 0.0);
	wheels->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(-0.6, 0.6, 0.0);
	wheels->draw(programID);
	modelview->pop();

	sendDataToShader(programID);
	modelview->pop();
}

void Bus::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Cube(o, game);
	wheels = new Sphere(o, game, 0.2, 8);
}
