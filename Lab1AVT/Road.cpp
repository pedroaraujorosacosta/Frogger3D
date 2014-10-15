#include "Cube.h"
#include "Road.h"
#include "Stack.h"
#include "Game.h"


Road::Road(float *position, Game *game) : Object(position, game)
{
	init();
	createBufferObjects();
}


Road::~Road()
{
	delete sideTop;
	delete road;
	delete sideBot;
}

void Road::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//modelview->rotateMatrix(0.0, 1.0, 0.0, 45.0);

	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);

	modelview->push();
	modelview->scaleMatrix(20.0, 5.0, 1.0);
	road->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->scaleMatrix(20.0, 1.0, 1.0);

	modelview->push();
	modelview->translateMatrix(0.0, 3, 0.0);
	sideTop->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.0, -3, 0.0);
	sideBot->draw(programID);
	modelview->pop();

	modelview->pop();

	//sendDataToShader(programID);
	modelview->pop();
}

void Road::init() {
	float o[3] = { 0.0, 0.0, 0.0 };

	sideTop = new Cube(o, this->game);
	road = new Cube(o, this->game);
	sideBot = new Cube(o, this->game);
}