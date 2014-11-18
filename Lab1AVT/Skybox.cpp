#include "Skybox.h"
#include "Quad.h"
#include "Game.h"
#include "MatrixStack.h"

Skybox::Skybox(float* position, Game *game) : Object(position, game)
{
	init();
}

Skybox::~Skybox()
{
	delete quad;
}

void Skybox::init()
{
	float o[3] = { 0.0f, 0.0f, 0.0f };
	quad = new Quad(o, game);

	float scale = 40.0f;
	float mat[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	quad->setAmbient(mat);
	quad->setDiffuse(mat);
	quad->setSpecular(mat);
	//quad->setScale(scale, scale, scale);
}

void Skybox::draw()
{
	
	float posLeft[3] = { -20.0f, 0.0f, -3.0f };
	float posRight[3] = { 20.0f, 0.0f, -3.0f };
	float posFront[3] = { 0.0f, 14.0f, -3.0f };
	float posBack[3] = { 0.0f, -14.0f, -3.0f };
	float posTop[3] = { 0.0f, 0.0f, 10.0f };
	float posBottom[3] = { 0.0f, 0.0f, -10.0f };

	MatrixStack *modelView = game->getModelViewStack();

	modelView->push();

	// draw left quad
	modelView->push();
	modelView->translateMatrix(posLeft[0], posLeft[1], posLeft[2]);
	modelView->rotateMatrix(0.0f, 0.0f, 1.0f, -90);
		quad->setTexCount(1);
		quad->setScale(28.0f, 1.0f, 28.0f);
		quad->draw();
	modelView->pop();

	// draw right quad
	modelView->push();
	modelView->translateMatrix(posRight[0], posRight[1], posRight[2]);
	modelView->rotateMatrix(0.0f, 0.0f, 1.0f, 90);
		quad->setTexCount(2);
		quad->setScale(28.0f, 1.0f, 28.0f);
		quad->draw();
	modelView->pop();

	// draw front quad
	modelView->push();
	modelView->translateMatrix(posFront[0], posFront[1], posFront[2]);
		quad->setScale(40.0f, 1.0f, 28.0f);
		quad->setTexCount(3);
		quad->draw();
	modelView->pop();

	// draw back quad
	modelView->push();
	modelView->translateMatrix(posBack[0], posBack[1], posBack[2]);
	modelView->rotateMatrix(0.0f, 0.0f, 1.0f, 180);
		quad->setTexCount(4);
		quad->setScale(40.0f, 1.0f, 28.0f);
		quad->draw();
	modelView->pop();

	// draw top quad
	modelView->push();
	modelView->translateMatrix(posTop[0], posTop[1], posTop[2]);
	modelView->rotateMatrix(1.0f, 0.0f, 0.0f, -90);
	quad->setTexCount(5);
		quad->setScale(40.0f, 1.0f, 28.0f);
		quad->draw();
	modelView->pop();

	modelView->pop();
}