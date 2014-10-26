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

void Road::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//modelview->rotateMatrix(0.0, 1.0, 0.0, 45.0);

	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);

	modelview->push();
	modelview->scaleMatrix(20.0, 5.0, 1.0);
	road->draw();
	modelview->pop();

	modelview->push();
	modelview->scaleMatrix(20.0, 1.0, 1.0);

	modelview->push();
	modelview->translateMatrix(0.0, 3, 0.0);
	sideTop->draw();
	modelview->pop();

	modelview->push();
	modelview->translateMatrix(0.0, -3, 0.0);
	sideBot->draw();
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

	// set materials
	float ambRoad[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffRoad[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float specRoad[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveRoad[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessRoad = 100.0f;
	int texcountRoad = 0;

	road->setAmbient(ambRoad);
	road->setDiffuse(diffRoad);
	road->setSpecular(specRoad);
	road->setEmissive(emissiveRoad);
	road->setShininess(shininessRoad);
	road->setTexCount(texcountRoad);

	float ambSide[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffSide[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float specSide[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveSide[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessSide = 100.0f;
	int texcountSide = 0;

	sideTop->setAmbient(ambSide);
	sideTop->setDiffuse(diffSide);
	sideTop->setSpecular(specSide);
	sideTop->setEmissive(emissiveSide);
	sideTop->setShininess(shininessSide);
	sideTop->setTexCount(texcountSide);

	sideBot->setAmbient(ambSide);
	sideBot->setDiffuse(diffSide);
	sideBot->setSpecular(specSide);
	sideBot->setEmissive(emissiveSide);
	sideBot->setShininess(shininessSide);
	sideBot->setTexCount(texcountSide);
}