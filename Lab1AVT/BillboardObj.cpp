#include "BillboardObj.h"
#include "Stack.h"
#include "Game.h"
#include "Camera.h"
#include "Vector.h"
#include "Stack.h"

#define M_PI 3.1415
#define radToDeg(x) ((x/M_PI)*180)

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

	Camera *cam = game->getCamera();
	Vector eyePos = cam->getEye();
	Vector objPos(position, 3);
	Vector objToEye = eyePos - objPos;
	float objToEyeProj[3] = { objToEye.v[0], objToEye.v[1], 0.0f };
	float projDir[3] = { direction[0], direction[1], 0.0f };
	Stack::normalize(objToEyeProj, 3);
	Stack::normalize(projDir, 3);
	float cosineAlpha = Stack::dotProduct(projDir, objToEyeProj, 3);
	float alpha = acos(cosineAlpha);
	float cross[3];

	if (objToEyeProj[0] > 0)
		Stack::crossProduct(projDir, objToEyeProj, cross);
	else {
		Stack::crossProduct(objToEyeProj, projDir, cross);
		cross[0] = -cross[0];
		cross[1] = -cross[1];
		cross[2] = -cross[2];
	}
	Stack::normalize(cross, 3);

	if (abs(cross[0]) < 0.01f && abs(cross[1]) < 0.01f)
	{
		//cross == up vector
		if (abs(cross[2] - 1.0f) < 0.01f)
			alpha = -alpha;
		else if (abs(cross[2] + 1.0f) < 0.01f)
			alpha = -(2 * M_PI - alpha);
	}

	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);

	//rotate to viewer position
	if(cam->isInFPSMode()) 
		modelview->rotateMatrix(0.0f, 0.0f, 1.0f, radToDeg(alpha));

	//puts the quad in the origin
	modelview->push();
	quad->setTexCount(mat.texCount);
		quad->draw();
	modelview->pop();

	/*modelview->push();
	//puts the quad in the origin
	modelview->rotateMatrix(0.0f, 0.0f, 1.0f, 180);
	quad->setTexCount(mat.texCount);
		quad->draw();
	modelview->pop();*/

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