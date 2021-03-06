#include "Sphere.h"
#include "MatrixStack.h"
#include "Game.h"



Sphere::Sphere(float* position, Game *game, float radius, int divisions): Object (position, game) {
	init();
	float *p = circularProfile(-3.14159f / 2.0f, 3.14159f / 2.0f, radius, divisions);
	computeVAO(divisions + 1, p + 2, p, divisions * 2, 0.0f);
	free(p);
}


void Sphere::draw(){

	MatrixStack* modelview = game->getModelViewStack();
	
	sendMaterials();

	modelview->push();
	//then, puts the sphere in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(scale[0], scale[1], scale[2]);
	
	sendDataToShader();
	modelview->pop();
}

void Sphere::init()
{

}