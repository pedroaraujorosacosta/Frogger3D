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


	//LANE 1
	float posFloatingLog1[] = { -7.0, 5.0, -2.5 };
	float dirFloatingLog1[] = { 0.05, 0.0, 0.0 };

	float posFloatingLog2[] = { 1.0, 5.0, -2.5 };
	float dirFloatingLog2[] = { 0.05, 0.0, 0.0 };

	float posFloatingLog3[] = { 8.0, 5.0, -2.5 };
	float dirFloatingLog3[] = { 0.05, 0.0, 0.0 };

	objectsMobileLane1.push_back(new FloatingLog(posFloatingLog1, game, getLane1Velocity(), dirFloatingLog1));
	objectsMobileLane1.push_back(new FloatingLog(posFloatingLog2, game, getLane1Velocity(), dirFloatingLog2));
	objectsMobileLane1.push_back(new FloatingLog(posFloatingLog3, game, getLane1Velocity(), dirFloatingLog3));

	//LANE 2

	float posTurtle1[] = { lateralDireita, 3.3, -2.5 };
	float dirTurtle1[] = { -0.05, 0.0, 0.0 };

	float posTurtle2[] = { 2.0, 3.3, -2.5 };
	float dirTurtle2[] = { -0.05, 0.0, 0.0 };

	float posTurtle3[] = { -4, 3.3, -2.5 };
	float dirTurtle3[] = { -0.05, 0.0, 0.0 };

	objectsMobileLane2.push_back(new Turtle(posTurtle1, game, getLane2Velocity(), dirTurtle1));
	objectsMobileLane2.push_back(new Turtle(posTurtle2, game, getLane2Velocity(), dirTurtle2));
	objectsMobileLane2.push_back(new Turtle(posTurtle3, game, getLane2Velocity(), dirTurtle3));

	//LANE 3

	float posFloatingLog4[] = { lateralEsquerda, 2.0, -2.5 };
	float dirFloatingLog4[] = { 0.05, 0.0, 0.0 };

	float posFloatingLog5[] = { 7.0, 2.0, -2.5 };
	float dirFloatingLog5[] = { 0.05, 0.0, 0.0 };

	float posFloatingLog6[] = { -2.0, 2.0, -2.5 };
	float dirFloatingLog6[] = { 0.05, 0.0, 0.0 };

	objectsMobileLane3.push_back(new FloatingLog(posFloatingLog4, game, getLane3Velocity(), dirFloatingLog4));
	objectsMobileLane3.push_back(new FloatingLog(posFloatingLog5, game, getLane3Velocity(), dirFloatingLog5));
	objectsMobileLane3.push_back(new FloatingLog(posFloatingLog6, game, getLane3Velocity(), dirFloatingLog6));

	//LANE 4

	float posBus[] = { lateralEsquerda, -1.8, -1.5 };
	float dirBus[] = { 0.05, 0.0, 0.0 };

	objectsMobileLane4.push_back(new Bus(posBus, game, getLane4Velocity(), dirBus));

	//LANE 5

	float posCar1[] = { lateralDireita, -3.5, -1.5 };
	float dirCar1[] = { -0.05, 0.0, 0.0 };

	float posCar2[] = { -2.0, -3.5, -1.5 };
	float dirCar2[] = { -0.05, 0.0, 0.0 };


	objectsMobileLane5.push_back(new Car(posCar1, game, getLane5Velocity(), dirCar1));
	objectsMobileLane5.push_back(new Car(posCar2, game, getLane5Velocity(), dirCar2));

	//LANE 6

	float posCar3[] = { 2.0, -5.0, -1.5 };
	float dirCar3[] = { 0.05, 0.0, 0.0 };

	float posCar4[] = { lateralEsquerda, -5.0, -1.5 };
	float dirCar4[] = { 0.05, 0.0, 0.0 };

	objectsMobileLane6.push_back(new Car(posCar3, game, getLane6Velocity(), dirCar3));
	objectsMobileLane6.push_back(new Car(posCar4, game, getLane6Velocity(), dirCar4));


}

ManagerObj::~ManagerObj() {

}

void ManagerObj::draw() {
	for (Object* o : objects)
		o->draw(game->getProgramID());
	for (MobileObj* o : objectsMobileLane1)
		o->draw(game->getProgramID());
	for (MobileObj* o : objectsMobileLane2)
		o->draw(game->getProgramID());
	for (MobileObj* o : objectsMobileLane3)
		o->draw(game->getProgramID());
	for (MobileObj* o : objectsMobileLane4)



		o->draw(game->getProgramID());
	for (MobileObj* o : objectsMobileLane5)
		o->draw(game->getProgramID());
	for (MobileObj* o : objectsMobileLane6)
		o->draw(game->getProgramID());

}

void ManagerObj::init(){


}

void ManagerObj::reset() {

}

void ManagerObj::update() {

	for (Object* o : objects){
		o->update();
	}

	for (MobileObj* o : objectsMobileLane1){
		o->update();
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane2){
		o->update();
		if (o->getPositionXXs() < (lateralEsquerda - 4))
			o->setPositionXXs(lateralDireita + 4 + (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane3){
		o->update();
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane4){
		o->update();
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane5){
		o->update();
		if (o->getPositionXXs() < (lateralEsquerda - 4))
			o->setPositionXXs(lateralDireita + 4 + (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane6){
		o->update();
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
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

float ManagerObj::getLane1Velocity() {

	return 1.5 * dificuldade;
}
float ManagerObj::getLane2Velocity() {

	return 0.5 * dificuldade;
}
float ManagerObj::getLane3Velocity() {

	return 1.5 * dificuldade;
}
float ManagerObj::getLane4Velocity() {

	return 1 * dificuldade;
}
float ManagerObj::getLane5Velocity() {

	return 2 * dificuldade;
}
float ManagerObj::getLane6Velocity() {

	return 2 * dificuldade;
}