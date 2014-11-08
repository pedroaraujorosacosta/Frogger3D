#include "BillboardObj.h"
#include "Stack.h"
#include "Game.h"

BillboardObj::BillboardObj(float *position, Game *game, float *direction, float width, float height) : Object(position, game)
{
	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];

	this->width = width;
	this->height = height;

	init();
	createBufferObjects();
}

BillboardObj::~BillboardObj()
{
	delete quad;
}

void BillboardObj::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	//puts the quad in the origin of the referencial
	modelview->translateMatrix(-0.5, 0.0, -0.5);

	modelview->push();
	quad->setTexCount(mat.texCount);
	quad->draw();
	modelview->pop();

	sendDataToShader();
	modelview->pop();
}

void BillboardObj::init() 
{
	quad = new Quad(position, game);

	// set materials
	float ambQuad[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diffQuad[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specQuad[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float emissiveQuad[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessQuad = 100.0f;
	int texcountQuad = 0;

	quad->setAmbient(ambQuad);
	quad->setDiffuse(diffQuad);
	quad->setSpecular(specQuad);
	quad->setEmissive(emissiveQuad);
	quad->setShininess(shininessQuad);
	quad->setTexCount(texcountQuad);
}

float BillboardObj::getDirX()
{
	return direction[0];
}

float BillboardObj::getDirY()
{
	return direction[1];
}

float BillboardObj::getDirZ()
{
	return direction[2];
}

float BillboardObj::getWidth()
{
	return width;
}