#include "Sphere.h"
#include "Stack.h"
#include "Game.h"



Sphere::Sphere(float* position, Game *game, float radius, int divisions): Object (position, game) {
	init();
	float *p = circularProfile(-3.14159f / 2.0f, 3.14159f / 2.0f, radius, divisions);
	computeVAO(divisions + 1, p + 2, p, divisions * 2, 0.0f);
	free(p);
}


void Sphere::draw(GLuint programID){

	Stack* modelview = game->getModelViewStack();
	
	modelview->push();
	//then, puts the sphere in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);

	sendDataToShader(programID);
	modelview->pop();
}

void Sphere::init()
{

	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.8f, 0.6f, 0.4f, 1.0f };
	float spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;

	// create geometry and VAO of the pawn
	memcpy(mat.ambient, amb, 4 * sizeof(float));
	memcpy(mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mat.specular, spec, 4 * sizeof(float));
	memcpy(mat.emissive, emissive, 4 * sizeof(float));
	mat.shininess = shininess;
	mat.texCount = texcount;
}