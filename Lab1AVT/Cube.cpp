#include "Cube.h"
#include "Stack.h"
#include "Game.h"

Cube::Cube(float *position, Game *game, bool isRepeatTexture, float factorX ) : Object(position, game)
{
	this->isRepeatTexture = isRepeatTexture;
	repeatFactorX = factorX;
	init();
	createBufferObjects();
}

void Cube::draw(){

	Stack* modelview = game->getModelViewStack();

	sendMaterials();

	modelview->push(); 
	//then, puts the unitary cube in the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	//puts the unitary cube in the origin of the referencial
	modelview->scaleMatrix(scale[0], scale[1], scale[2]);
	modelview->translateMatrix(-0.5, -0.5, -0.5);

	sendDataToShader();
	modelview->pop();
}

void Cube::init() {
	this->verticeCount = 24;
	this->faceCount = 12;

	float vertex[] = {
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f
	};

	this->vertices = new float[4*verticeCount];

	for (int i = 0; i < verticeCount*4; i++) {
		vertices[i] = vertex[i];
	}
	
	float norm[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	this->normals = new float[3 * verticeCount];

	for (int i = 0; i < verticeCount * 3; i++) {
		normals[i] = norm[i];
	}

	float texs[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	float texsRepeatCube[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		repeatFactorX, 0.0f,
		repeatFactorX, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		repeatFactorX, 0.0f,
		repeatFactorX, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		repeatFactorX, 0.0f,
		repeatFactorX, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		repeatFactorX, 0.0f,
		repeatFactorX, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		repeatFactorX, 0.0f,
		repeatFactorX, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		repeatFactorX, 0.0f,
		repeatFactorX, 1.0f,
	};

	this->texCoords = new float[2 * verticeCount];

	if (!isRepeatTexture) {
		for (int i = 0; i < verticeCount * 2; i++) {
			texCoords[i] = texs[i];
		}
	}
	else {
		for (int i = 0; i < verticeCount * 2; i++) {
			texCoords[i] = texsRepeatCube[i];
		}
	}

	unsigned int faces[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	this->faceIndex = new unsigned int[3 * faceCount];

	for (int i = 0; i < faceCount * 3; i++) {
		faceIndex[i] = faces[i];
	}
}