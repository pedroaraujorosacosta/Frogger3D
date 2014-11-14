#include "Frog.h"
#include "Stack.h"
#include "Game.h"
#include <iostream>
#include "Light.h"
#include "Vector.h"
#include "ManagerObj.h"
#include "Vector.h"

Frog::Frog(float *position, Game *game, float velocity, float *direction, int life) : MobileObj(position, game, velocity, direction, 1.4f, 1.0f)
{
	init();
	createBufferObjects();
	this->life = life;
}

Frog::~Frog()
{
	delete body;
	delete eyes;
}

void Frog::update(float dt)
{
	Vector laneDir(3);
	ManagerObj *managerObj = game->getManagerObj();
	float laneVel = managerObj->getRiverLaneVelocity(laneDir);


	//ZZs have no use for the boundaries
	position[2] += direction[2] * velocity + laneDir.v[2] * laneVel * dt;

	//Boundaries check before update
	float newXXsPosition = position[0] + direction[0] * velocity + laneDir.v[0] * laneVel * dt;
	float newYYsPosition = position[1] + direction[1] * velocity + laneDir.v[1] * laneVel * dt;

	// Check bounds on the direction it needs to go
	if (this->direction[0] == 1){

		if (newXXsPosition >= 19)
			this->velocity = 0; //stop and dont update the position
		else if ((newXXsPosition >= 8.4) && (position[0] < 8.4) &&
						 				( ((position[1] > -2.1) && (position[1] < 0.5)) ||
										  (position[1] < -11.4)))
			this->velocity = 0; //stop and dont update the position
		else if ((newXXsPosition >= -1.2) && (position[0] < 1.2) &&
										( ((position[1] > -2.1) && (position[1] < 0.5)) ||
									   	  (position[1] < -11.4)))
			this->velocity = 0; //stop and dont update the position
		else if ((newXXsPosition >= -11.4) && (position[0] < -11.4) &&
										( ((position[1] > -2.1) && (position[1] < 0.5)) ||
										  (position[1] < -11.4)))
			this->velocity = 0; //stop and dont update the position
		else{
			position[0] = newXXsPosition;//if boundaries check out, update
			position[1] = newYYsPosition;
		}
	
	}
	else if (this->direction[0] == -1){
	
		if (newXXsPosition <= -19)
			this->velocity = 0; //stop and dont update the position
		else if ((newXXsPosition <= -8.4) && (position[0] > -8.4) &&
										( ((position[1] > -2.1) && (position[1] < 0.5)) ||
										  (position[1] < -11.4)))
			this->velocity = 0; //stop and dont update the position
		else if ((newXXsPosition <= 1.2) && (position[0] > 1.2) &&
										( ((position[1] > -2.1) && (position[1] < 0.5)) ||
										  (position[1] < -11.4)))
			this->velocity = 0; //stop and dont update the position
		else if ((newXXsPosition <= 11.4) && (position[0] > 11.4) &&
										( ((position[1] > -2.1) && (position[1] < 0.5)) ||
										  (position[1] < -11.4)))
			this->velocity = 0; //stop and dont update the position
		else{
			position[0] = newXXsPosition;//if boundaries check out, update
			position[1] = newYYsPosition;
		}
	}
	else if (this->direction[1] == 1){

		if (newYYsPosition >= 12 && !game->isGameWon())
			game->winGame(); //game has been won!
		else if ((newYYsPosition >= -2.1) && (position[1] < -2.1) && 
										 ( ((position[0] > 8.4) && (position[0] < 11.4)) || 
										   ((position[0] > -1.5) && (position[0] < 1.2)) ||
										   ((position[0] > -11.4) && (position[0] < -8.4)) ))
			this->velocity = 0; //stop and dont update the position
		else{
			position[0] = newXXsPosition;//if boundaries check out, update
			position[1] = newYYsPosition;
		}
	}
	else if (this->direction[1] == -1){
	
		if (newYYsPosition <= -13.5)
			this->velocity = 0; //stop and dont update the position
		else if ((newYYsPosition <= 0.5) && (position[1] > 0.5) &&
								( ((position[0] > 8.4) && (position[0] < 11.4)) ||
								  ((position[0] > -1.5) && (position[0] < 1.2)) ||
								  ((position[0] > -11.4) && (position[0] < -8.4))))
			this->velocity = 0; //stop and dont update the position
		else if ((newYYsPosition <= -11.4) && (position[1] > -11.4) &&
								( ((position[0] > 8.4) && (position[0] < 11.4)) ||
								  ((position[0] > -1.5) && (position[0] < 1.2)) ||
								  ((position[0] > -11.4) && (position[0] < -8.4))))
			this->velocity = 0; //stop and dont update the position
		else{
			position[0] = newXXsPosition;//if boundaries check out, update
			position[1] = newYYsPosition;
		}
	}



	//if there is movement compute light
	if (velocity > 0.0f || fabs(laneVel) > 0.0f)
	{
		// adjust the miner's spot light
		Light *spot = game->getSpotLight();
		
		float newSpotDirf[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++) newSpotDirf[i] = direction[i];
		Vector newSpotDir(newSpotDirf, 4);
		newSpotDir.normalize();

		float hNewSpotPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		for (int i = 0; i < 3; i++) hNewSpotPos[i] = position[i] + 0.5*direction[i];
		Vector newSpotPos(hNewSpotPos, 4);
		spot->setPosition(newSpotPos);
	
	}
}

void Frog::move(float *direction)
{
	Light *spot = game->getSpotLight();
	float newSpotDirf[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 3; i++) newSpotDirf[i] = direction[i];
	Vector newSpotDir(newSpotDirf, 4);
	newSpotDir.normalize();
	spot->setDirection(newSpotDir);

	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];
	velocity = FROG_VELOCITY;
}

void Frog::move()
{
	velocity = FROG_VELOCITY;
}

void Frog::stop() 
{
	velocity = 0.0;
}

void Frog::draw(){

	Stack* modelview = game->getModelViewStack();

	modelview->push();
	//puts the frog on the right position
	modelview->translateMatrix(this->position[0], this->position[1], this->position[2]);
	modelview->scaleMatrix(1.2, 1.0, 0.8);  
	
	if (direction[0] < 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, -90);
	else if (direction[0] > 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 90);
	else if (direction[1] < 0.0)
		modelview->rotateMatrix(0.0, 0.0, 1.0, 180);

	body->draw();
	
	modelview->push();
	modelview->translateMatrix(0.3, 0.6, 0.0);
	eyes->draw();
	modelview->pop();
	
	modelview->push();
	modelview->translateMatrix(-0.3, 0.6, 0.0);
	eyes->draw();
	modelview->pop();

	sendDataToShader();
	modelview->pop();
}

void Frog::init() {
	float o[3] = { 0.0, 0.0, 0.0 };
	body = new Sphere(o, game, 0.6, 6);
	eyes = new Sphere(o, game, 0.2, 6);

	// set materials
	float ambBody[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diffBody[] = { 0.1f, 0.6f, 0.1f, 1.0f };
	float specBody[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissiveBody[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessBody = 100.0f;
	int texcountBody = 0;

	body->setAmbient(ambBody);
	body->setDiffuse(diffBody);
	body->setSpecular(specBody);
	body->setEmissive(emissiveBody);
	body->setShininess(shininessBody);
	body->setTexCount(texcountBody);

	float ambEyes[] = { 0.1f, 0.2f, 0.1f, 1.0f };
	float diffEyes[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specEyes[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissiveEyes[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininessEyes = 100.0f;
	int texcountEyes = 0;

	eyes->setAmbient(ambEyes);
	eyes->setDiffuse(diffEyes);
	eyes->setSpecular(specEyes);
	eyes->setEmissive(emissiveEyes);
	eyes->setShininess(shininessEyes);
	eyes->setTexCount(texcountEyes);

	Light *spot = game->getSpotLight();
	if (spot != 0) 
	{
		float newSpotDirf[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++) newSpotDirf[i] = direction[i];
		Vector newSpotDir(newSpotDirf, 4);
		newSpotDir.normalize();
		spot->setDirection(newSpotDir);

		float hNewSpotPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		for (int i = 0; i < 3; i++) hNewSpotPos[i] = position[i] + 0.5*direction[i];
		Vector newSpotPos(hNewSpotPos, 4);
		spot->setPosition(newSpotPos);
	}
}

int Frog::getLife()
{
	return life;
}

void Frog::setLife(int life)
{
	this->life = life;
}


void Frog::killed()
{
	setLife(getLife() - 1);
	setPositionXXs(3.0);
	setPositionYYs(-13.0);
	setPositionZZs(-2.0);
}

void Frog::setPosition(float* pos)
{
	for (int i = 0; i < 3; i++)
		this->position[i] = pos[i];
}
void Frog::setDirection(float* dir)
{
	for (int i = 0; i < 3; i++)
		this->direction[i] = dir[i];
}
void Frog::setVelocity(float v)
{
	this->velocity = v;
}