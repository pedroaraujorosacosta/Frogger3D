#include "Cube.h"
#include "River.h"
#include "Stack.h"
#include "Game.h"


River::River(float *position, Game *game) : Object(position, game)
{
	init();
	createBufferObjects();
}

River::~River()
{
	delete bankTop;
	delete river;
	delete bankBot;
}

void River::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//modelview->rotateMatrix(0.0, 1.0, 0.0, 45.0);

	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);

	modelview->push();
	modelview->scaleMatrix(20.0, 5.0, 1.0);
		river->draw(programID);
	modelview->pop();
	
	modelview->push();
	modelview->scaleMatrix(20.0, 1.0, 1.0);

	modelview->push();
		modelview->translateMatrix(0.0, 3.0, 0.0);
		bankTop->draw(programID);
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.0, -3.0, 0.0);
		bankBot->draw(programID);
	modelview->pop();
	
	modelview->pop();

	//sendDataToShader(programID);
	modelview->pop();
}

void River::init() {
	float o[3] = { 0.0, 0.0, 0.0 };

	bankTop = new Cube(o, this->game);
	river = new Cube(o, this->game);
	bankBot = new Cube(o, this->game);
}
