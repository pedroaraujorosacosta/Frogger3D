#include "BillboardObj.h"
#include "Stack.h"
#include "Game.h"
#include "Camera.h"

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
	float* cam = game->getCamera()->getCameraPosition();

	float lookAt[3], objToCamProj[3], upAux[3];
	float angleCosine = 1;

	// objToCamProj is the vector in world coordinates from the 
	// local origin to the camera projected in the XY plane
	objToCamProj[0] = cam[0] - this->getPositionXXs();
	objToCamProj[1] = cam[1] - this->getPositionYYs();
	objToCamProj[2] = 0;


	// This is the original lookAt vector for the object 
	// in world coordinates
	lookAt[0] = 0;
	lookAt[1] = -1;
	lookAt[2] = 0;


	modelview->normalize(objToCamProj, 3);

	angleCosine = modelview->dotProduct(objToCamProj, lookAt, 3);


	modelview->crossProduct(objToCamProj, lookAt, upAux);

	modelview->push();
	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);


	modelview->push();
	//puts the quad in the originif ((angleCosine < 0.99990) && (angleCosine > -0.9999))
	if ((angleCosine < 0.99990) && (angleCosine > -0.99990))
		modelview->rotateMatrix(upAux[0], upAux[1], upAux[2], acos(angleCosine) * 180 / 3.14);
	
	quad->setTexCount(mat.texCount);
		quad->draw();
	modelview->pop();

	sendDataToShader();
	modelview->pop();
}

void BillboardObj::init() 
{
	float o[3] = { 0.0f, 0.0f, 0.0f };
	quad = new Quad(o, game);

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
