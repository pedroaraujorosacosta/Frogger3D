#include "Sphere.h"
#include "Stack.h"
#include "Game.h"



Sphere::Sphere(float* position, Game *game, float radius, int divisions): Object (position, game) {

	float *p = circularProfile(-3.14159f / 2.0f, 3.14159f / 2.0f, radius, divisions);
	computeVAO(divisions + 1, p + 2, p, divisions * 2, 0.0f);
}


void Sphere::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();
	//static int angle = 0;
	modelview->push();
	//then, puts the sphere in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	//modelview->rotateMatrix(0, 1, 0, angle++);

	sendDataToShader(programID);
	modelview->pop();
}

void Sphere::init()
{

}