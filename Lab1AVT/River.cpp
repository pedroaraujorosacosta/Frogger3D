#include "Cube.h"
#include "River.h"
#include "MatrixStack.h"
#include "Game.h"
#include <algorithm>


River::River(float *position, Game *game) : Object(position, game)
{
	init();
	createBufferObjects();
}

River::~River()
{
	for (Object *o : objects)
		delete o;
}

void River::draw(){

	MatrixStack* modelview = game->getModelViewStack();

	modelview->push();

	modelview->translateMatrix(position[0], position[1], position[2]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (std::vector<Object*>::reverse_iterator it = objects.rbegin(); it != objects.rend(); it++)
		(*it)->draw();
	glDisable(GL_BLEND);

	modelview->pop();
}

void River::update(float dt)
{
	std::sort(objects.begin(), objects.end(), comparatorObjects);
}

void River::init() {
	// init river
	float riverPos[3] = { 0.0f, 0.0f, -0.5f };
	Cube *river = new Cube(riverPos, game);
	river->setScale(40.0, 10.0, 1.0);
	objects.push_back(river);

	// init river water
	float riverWaterPos[3] = { 0.0f, 0.0f, 0.0f };
	riverWater = new Cube(riverWaterPos, game);
	riverWater->setScale(40.0, 10.0, 1.0);
	objects.push_back(riverWater);
	
	// init river banks
	float bankTopPos[3] = { 0.0f, 6.0f, 0.0f };
	Cube *bankTop = new Cube(bankTopPos, game, true, 10.0f);
	bankTop->setScale(40.0, 2.0, 1.0);
	objects.push_back(bankTop);

	float bankBotPos[3] = { 0.0f, -6.0f, 0.0f };
	Cube *bankBot = new Cube(bankBotPos, game, true, 10.0f);
	bankBot->setScale(40.0, 2.0, 1.0);
	objects.push_back(bankBot);
	

	// set materials

	// for river
	float ambRiver[] = { 0.1f, 0.5f, 1.0f, 1.0f };
	float diffRiver[] = { 0.1f, 0.5f, 1.0f, 1.0f };
	float specRiver[] = { 0.1f, 0.5f, 1.0f, 1.0f };
	float emissiveRiver[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float shininessRiver = 100.0f;
	int texcountRiver = 2;

	river->setAmbient(ambRiver);
	river->setDiffuse(diffRiver);
	river->setSpecular(specRiver);
	river->setEmissive(emissiveRiver);
	river->setShininess(shininessRiver);
	river->setTexCount(texcountRiver);

	// for river water
	float matRiverWater[] = { 1.0f, 1.0f, 1.0f, 0.05f };
	int texCountRiverWater = 1;
	riverWater->setAmbient(matRiverWater);
	riverWater->setDiffuse(matRiverWater);
	riverWater->setSpecular(matRiverWater);
	riverWater->setTexCount(texCountRiverWater);


	float ambBank[] = { 0.1f, 0.2f, 0.1f, 1.0f };
	float diffBank[] = { 0.1f, 0.7f, 0.1f, 1.0f };
	float specBank[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveBank[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBank = 100.0f;
	int texcountBank = 2;

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
