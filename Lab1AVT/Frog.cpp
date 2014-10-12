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

void Frog::update(){

	for (int i = 0; i < 3; i++)
		position[i] += direction[i] * velocity;
}

void Frog::move(float *direction){
	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];
	velocity = 0.08;
}

void Frog::stop() 
{
	velocity = 0.0;
}

void Frog::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the frog on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(2.0, 2.0, 1.0);  
	
	if (direction[0] < 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, -90);
	else if (direction[0] > 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);
	else if (direction[1] < 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 180);

	body->draw(programID);
	
	modelview->push();
	modelview->translateMatrix(0.3, 0.6, 0.0);
	eyes->draw(programID);
	modelview->pop();
	
	modelview->push();
	modelview->translateMatrix(-0.3, 0.6, 0.0);
	eyes->draw(programID);
	modelview->pop();

	sendDataToShader(programID);
	modelview->pop();
}

void Frog::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Sphere(o, game, 0.6, 8);
	eyes = new Sphere(o, game, 0.2, 8);
}
