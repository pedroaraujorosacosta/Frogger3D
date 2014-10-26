#include "Blocker.h"
#include "Stack.h"
#include "Game.h"


Blocker::Blocker(float *position, Game *game, float velocity, float *direction) : MobileObj(position, game, velocity, direction)
{
	init();
	createBufferObjects();
}

void Blocker::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	//puts the unitary cube in the origin of the referencial
	modelview->translateMatrix(-0.5, -0.5, -0.5);

	sendDataToShader();
	modelview->pop();
}

void Blocker::init() {


}
