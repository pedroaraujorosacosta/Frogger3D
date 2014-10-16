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
}
