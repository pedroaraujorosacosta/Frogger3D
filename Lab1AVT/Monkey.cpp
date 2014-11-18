#include "Monkey.h"
#include "MatrixStack.h"
#include "Game.h"

Monkey::Monkey(float *position, Game *game, bool isRepeatTexture, float factorX) : Object(position, game)
{
	init();
	createBufferObjects();
}

void Monkey::draw(){

	MatrixStack* modelview = game->getModelViewStack();

	sendMaterials();

	modelview->push();
	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	//puts the unitary cube in the origin of the referencial
	modelview->scaleMatrix(2.0f, 2.0f, 2.0f);

	sendDataToShader();
	modelview->pop();
}

void Monkey::init() 
{
	loadObjectFromFile("monkey.obj");
}