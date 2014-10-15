#include <iostream>
#include <string>

#include "Object.h"
#include "Game.h"

#include "River.h"
#include "Road.h"
#include "Turtle.h"
#include "FloatingLog.h"
#include "Bus.h"
#include "Car.h"

#include "ManagerObj.h"


ManagerObj::ManagerObj(Game *game) : Manager(game) {

	setDificuldade(2);
	setLateralEsquerda(-10.0);
	setLateralDireita(10.0);


	float posRiver[] = { 0.0, 3.5, -3.0 };
	objects.push_back(new River(posRiver, game));
	float posroad[] = { 0.0, -3.5, -3.0 };
	objects.push_back(new Road(posroad, game));


	float posBus[] = { lateralEsquerda, -1.6, -1.0 };
	float dirBus[] = { 0.05, 0.0, 0.0 };
	float velBus = getBusVelocity();

	float posCar1[] = { lateralDireita, -3.5, -1.0 };
	float dirCar1[] = { -0.05, 0.0, 0.0 };
	float velCar1 = getCarVelocity();;

	float posCar2[] = { 0.0, -5.0, -1.0 };
	float dirCar2[] = { 0.05, 0.0, 0.0 };
	float velCar2 = getCarVelocity();;

	float posCar3[] = { 1.0, -3.5, -1.0 };
	float dirCar3[] = { -0.05, 0.0, 0.0 };
	float velCar3 = getCarVelocity();;

	float posCar4[] = { lateralEsquerda, -5.0, -1.0 };
	float dirCar4[] = { 0.05, 0.0, 0.0 };
	float velCar4 = getCarVelocity();;

	//float posTurtle[] = { 0.0, 0.0, 0.0 };
	//float posFloatingLog[] = { 0.0, 0.0, 0.0 };

	float posFloatingLog[] = { lateralEsquerda, 5.0, -2.0 };
	float dirFloatingLog[] = { 0.05, 0.0, 0.0 };
	float velFloatingLog = getFloatingLogVelocity();;

	objectsMobile.push_back(new Bus(posBus, game, velBus, dirBus));
	objectsMobile.push_back(new Car(posCar1, game, velCar1, dirCar1));
	objectsMobile.push_back(new Car(posCar2, game, velCar2, dirCar2));
	objectsMobile.push_back(new Car(posCar3, game, velCar3, dirCar3));
	objectsMobile.push_back(new Car(posCar4, game, velCar4, dirCar4));

	objectsMobile.push_back(new FloatingLog(posFloatingLog, game, velFloatingLog, dirFloatingLog));
	//objects.push_back(new Sphere(posroad, this, 3, 100));


}

ManagerObj::~ManagerObj() {

}

void ManagerObj::draw() {
	for (int i = 0; i < objects.size(); i++)
		objects[i]->draw(game->getProgramID());
	for (int i = 0; i < objectsMobile.size(); i++)
		objectsMobile[i]->draw(game->getProgramID());

}

void ManagerObj::init(){


}

void ManagerObj::reset() {

}

void ManagerObj::update() {

	for (int i = 0; i < objects.size(); i++){
		objects[i]->update();
	}

	for (int i = 0; i < objectsMobile.size(); i++){
		objectsMobile[i]->update();
		if (objectsMobile[i]->goesLeft()){
			if (objectsMobile[i]->getPositionXXs() < (lateralEsquerda - 4))
				objectsMobile[i]->setPositionXXs(lateralDireita + 4);
		}
		else if (objectsMobile[i]->getPositionXXs() > (lateralDireita + 4))
			objectsMobile[i]->setPositionXXs(lateralEsquerda - 4);
	}

}


void ManagerObj::setDificuldade(float dificuldade){
	this->dificuldade = dificuldade;
}

void ManagerObj::setLateralEsquerda(float lateralEsquerda){
	this->lateralEsquerda = lateralEsquerda;
}

void ManagerObj::setLateralDireita(float lateralDireita){
	this->lateralDireita = lateralDireita;
}

float ManagerObj::getBusVelocity() {

	return 1 * dificuldade;
}
float ManagerObj::getCarVelocity() {

	return 2 * dificuldade;
}
float ManagerObj::getFloatingLogVelocity() {

	return 1 * dificuldade;
}
float ManagerObj::getTurtleVelocity() {

	return 1 * dificuldade;
}