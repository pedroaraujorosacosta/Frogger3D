#include "MobileObj.h"
#include "Stack.h"
#include "Game.h"

MobileObj::MobileObj(float *position, Game *game, float velocity, float *direction) : Object(position, game)
{
	this->velocity = velocity;
	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];

	init();
	createBufferObjects();
}

void MobileObj::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	//puts the unitary cube in the origin of the referencial
	modelview->translateMatrix(-0.5, -0.5, -0.5);

	sendDataToShader(programID);
	modelview->pop();
}

void MobileObj::init() {


}
