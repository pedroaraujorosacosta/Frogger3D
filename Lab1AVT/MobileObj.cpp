#include "MobileObj.h"
#include "Stack.h"
#include "Game.h"

MobileObj::MobileObj(float *position, Game *game, float velocity, float *direction, float width, float height) : Object(position, game)
{
	initVelocity = this->velocity = velocity;
	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];

	this->width = width;
	this->height = height;

	init();
	createBufferObjects();
}

void MobileObj::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	//puts the unitary cube in the origin of the referencial
	modelview->translateMatrix(-0.5, -0.5, -0.5);

	sendDataToShader();
	modelview->pop();
}

void MobileObj::reset() {
	this->velocity = initVelocity;
}

void MobileObj::init() {


}

float MobileObj::getDirX()
{
	return direction[0];
}

float MobileObj::getDirY()
{
	return direction[1];
}

float MobileObj::getDirZ()
{
	return direction[2];

}

void MobileObj::multiplyVelocity(float dificulty)
{
	velocity = velocity + dificulty * initVelocity / 500;
}

float MobileObj::getWidth()
{
	return width;
}

float MobileObj::getVelocity()
{
	return velocity;
}