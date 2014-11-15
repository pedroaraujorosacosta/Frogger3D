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
	delete side;
	delete road;
	delete lampPost;
	delete lampadaTopo;
}

void Road::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//modelview->rotateMatrix(0.0, 1.0, 0.0, 45.0);

	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);

	modelview->push();
	modelview->scaleMatrix(40.0, 10.0, 1.0);
	road->setTexCount(2);
	road->draw();
	modelview->pop();

	modelview->push();
	modelview->scaleMatrix(40.0, 2.0, 1.0);

		modelview->push();
		modelview->translateMatrix(0.0, 3, 0.0);
		side->draw();
		modelview->pop();

		modelview->push();
		modelview->translateMatrix(0.0, -3, 0.0);
		side->draw();
		modelview->pop();
	
	modelview->pop();


	modelview->push();
	modelview->scaleMatrix(1.0, 1.0, 2.0);

		modelview->push();
		modelview->translateMatrix(0.0, 6.0, 0.6);
		lampPost->draw();
			modelview->push();
			modelview->scaleMatrix(1.0, 1.0, 0.5);
			modelview->translateMatrix(0.0, 0.0, 0.5);
			lampadaTopo->draw();
			modelview->pop();
		modelview->pop();

		modelview->push();
		modelview->translateMatrix(10.0, 6.0, 0.6);
		lampPost->draw();
			modelview->push();
			modelview->scaleMatrix(1.0, 1.0, 0.5);
			modelview->translateMatrix(0.0, 0.0, 0.5);
			lampadaTopo->draw();
			modelview->pop();
		modelview->pop();

		modelview->push();
		modelview->translateMatrix(-10.0, 6.0, 0.6);
		lampPost->draw();
			modelview->push();
			modelview->scaleMatrix(1.0, 1.0, 0.5);
			modelview->translateMatrix(0.0, 0.0, 0.5);
			lampadaTopo->draw();
			modelview->pop();
		modelview->pop();

		modelview->push();
		modelview->translateMatrix(0.0, -6.0, 0.6);
		lampPost->draw();
			modelview->push();
			modelview->scaleMatrix(1.0, 1.0, 0.5);
			modelview->translateMatrix(0.0, 0.0, 0.5);
			lampadaTopo->draw();
			modelview->pop();
		modelview->pop();

		modelview->push();
		modelview->translateMatrix(10.0, -6.0, 0.6);
		lampPost->draw();
			modelview->push();
			modelview->scaleMatrix(1.0, 1.0, 0.5);
			modelview->translateMatrix(0.0, 0.0, 0.5);
			lampadaTopo->draw();
			modelview->pop();
		modelview->pop();

		modelview->push();
		modelview->translateMatrix(-10.0, -6.0, 0.6);
		lampPost->draw();
			modelview->push();
			modelview->scaleMatrix(1.0, 1.0, 0.5);
			modelview->translateMatrix(0.0, 0.0, 0.5);
			lampadaTopo->draw();
			modelview->pop();
		modelview->pop();

	modelview->pop();

	
	//sendDataToShader(programID);
	modelview->pop();
}

void Road::init() {
	float o[3] = { 0.0, 0.0, 0.0 };

	side = new Cube(o, this->game);
	road = new Cube(o, this->game, true, 2.0f);
	lampPost = new Cube(o, this->game);
	lampadaTopo = new Sphere(o, this->game, 1, 4);

	// set materials
	float ambRoad[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffRoad[] = { 0.2f, 0.2f, 0.2f, 1.0f };
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
	float diffSide[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float specSide[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissiveSide[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessSide = 100.0f;
	int texcountSide = 0;

	side->setAmbient(ambSide);
	side->setDiffuse(diffSide);
	side->setSpecular(specSide);
	side->setEmissive(emissiveSide);
	side->setShininess(shininessSide);
	side->setTexCount(texcountSide);


	float ambPoste[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffPoste[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float specPoste[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissivePoste[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessPoste = 100.0f;
	int texcountPoste = 0;
	
	lampPost->setAmbient(ambPoste);
	lampPost->setDiffuse(diffPoste);
	lampPost->setSpecular(specPoste);
	lampPost->setEmissive(emissivePoste);
	lampPost->setShininess(shininessPoste);
	lampPost->setTexCount(texcountPoste);

	
	
	float ambPost2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffPost2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float specPost2[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float emissivePost2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessPost2 = 100.0f;
	int texcountPost2 = 0;

	
	lampadaTopo->setAmbient(ambPost2);
	lampadaTopo->setDiffuse(diffPost2);
	lampadaTopo->setSpecular(specPost2);
	lampadaTopo->setEmissive(emissivePost2);
	lampadaTopo->setShininess(shininessPost2);
	lampadaTopo->setTexCount(texcountPost2);

}