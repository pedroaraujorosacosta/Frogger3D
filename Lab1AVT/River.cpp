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

	// set materials
	float ambRiver[] = { 0.1f, 0.1f, 0.2f, 1.0f };
	float diffRiver[] = { 0.1f, 0.1f, 0.8f, 1.0f };
	float specRiver[] = { 0.1f, 0.1f, 0.8f, 1.0f };
	float emissiveRiver[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessRiver = 100.0f;
	int texcountRiver = 0;

	river->setAmbient(ambRiver);
	river->setDiffuse(diffRiver);
	river->setSpecular(specRiver);
	river->setEmissive(emissiveRiver);
	river->setShininess(shininessRiver);
	river->setTexCount(texcountRiver);

	float ambBank[] = { 0.1f, 0.2f, 0.1f, 1.0f };
	float diffBank[] = { 0.1f, 0.9f, 0.1f, 1.0f };
	float specBank[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveBank[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBank = 100.0f;
	int texcountBank = 0;

	bankTop->setAmbient(ambBank);
	bankTop->setDiffuse(diffBank);
	bankTop->setSpecular(specBank);
	bankTop->setEmissive(emissiveBank);
	bankTop->setShininess(shininessBank);
	bankTop->setTexCount(texcountBank);

	bankBot->setAmbient(ambBank);
	bankBot->setDiffuse(diffBank);
	bankBot->setSpecular(specBank);
	bankBot->setEmissive(emissiveBank);
	bankBot->setShininess(shininessBank);
	bankBot->setTexCount(texcountBank);
}
