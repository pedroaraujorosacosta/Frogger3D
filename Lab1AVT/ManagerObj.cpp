#include <iostream>
#include <string>

#include "Object.h"
#include "Game.h"
#include "Frog.h"

#include "River.h"
#include "Road.h"
#include "Turtle.h"
#include "FloatingLog.h"
#include "Bus.h"
#include "Car.h"

#include "ManagerObj.h"


ManagerObj::ManagerObj(Game *game) : Manager(game) {

	setDificuldade(1);
	setLateralEsquerda(-20.0);
	setLateralDireita(20.0);

	//collision variables init
	carWeigth = 1.2;
	busWeigth = 2.4;
	logWeigth = 2.2;
	turtleWeigth = 2.0;
	frogHeigth = 1.0;
	frogWeigth = 1.4;


	float posRiver[] = { 0.0, 7, -3.0 };
	objects.push_back(new River(posRiver, game));
	float posroad[] = { 0.0, -7, -3.0 };
	objects.push_back(new Road(posroad, game));


	//LANE 1
	float posFloatingLog1[] = { -5.0, 11.0, -2.5 };
	float dirFloatingLog1[] = { 1, 0.0, 0.0 };

	float posFloatingLog2[] = { 1.0, 11.0, -2.5 };
	float dirFloatingLog2[] = { 1, 0.0, 0.0 };

	float posFloatingLog3[] = { 8.0, 11.0, -2.5 };
	float dirFloatingLog3[] = { 1, 0.0, 0.0 };

	objectsMobileLane1.push_back(new FloatingLog(posFloatingLog1, game, getLane1Velocity(), dirFloatingLog1));
	objectsMobileLane1.push_back(new FloatingLog(posFloatingLog2, game, getLane1Velocity(), dirFloatingLog2));
	objectsMobileLane1.push_back(new FloatingLog(posFloatingLog3, game, getLane1Velocity(), dirFloatingLog3));

	//LANE 2

	float posTurtle1[] = { lateralDireita, 9, -2.5 };
	float dirTurtle1[] = { -1, 0.0, 0.0 };

	float posTurtle2[] = { 2.0, 9, -2.5 };
	float dirTurtle2[] = { -1, 0.0, 0.0 };

	float posTurtle3[] = { -4, 9, -2.5 };
	float dirTurtle3[] = { -1, 0.0, 0.0 };

	objectsMobileLane2.push_back(new Turtle(posTurtle1, game, getLane2Velocity(), dirTurtle1));
	objectsMobileLane2.push_back(new Turtle(posTurtle2, game, getLane2Velocity(), dirTurtle2));
	objectsMobileLane2.push_back(new Turtle(posTurtle3, game, getLane2Velocity(), dirTurtle3));

	//LANE 3

	float posFloatingLog4[] = { lateralEsquerda, 7.0, -2.5 };
	float dirFloatingLog4[] = { 1, 0.0, 0.0 };

	float posFloatingLog5[] = { 7.0, 7.0, -2.5 };
	float dirFloatingLog5[] = { 1, 0.0, 0.0 };

	float posFloatingLog6[] = { -2.0, 7.0, -2.5 };
	float dirFloatingLog6[] = { 1, 0.0, 0.0 };

	objectsMobileLane3.push_back(new FloatingLog(posFloatingLog4, game, getLane3Velocity(), dirFloatingLog4));
	objectsMobileLane3.push_back(new FloatingLog(posFloatingLog5, game, getLane3Velocity(), dirFloatingLog5));
	objectsMobileLane3.push_back(new FloatingLog(posFloatingLog6, game, getLane3Velocity(), dirFloatingLog6));

	//BRIDGE new Lane 4

	float posFloatingLog7[] = { 0.0, 5.0, -2.5 };
	float dirFloatingLog7[] = { 1, 0.0, 0.0 };


	objectsMobileLane4.push_back(new FloatingLog(posFloatingLog7, game, getLane3Velocity(), dirFloatingLog4));

	//BRIDGE new Lane 5

	float posFloatingLog8[] = { 0.0, 3.0, -2.5 };
	float dirFloatingLog8[] = { 1, 0.0, 0.0 };

	objectsMobileLane5.push_back(new FloatingLog(posFloatingLog8, game, getLane3Velocity(), dirFloatingLog4));

	//LANE 4

	float posBus[] = { lateralEsquerda, -3, -1.8 };
	float dirBus[] = { 1, 0.0, 0.0 };

	objectsMobileLane6.push_back(new Bus(posBus, game, getLane4Velocity(), dirBus));

	//LANE 5

	float posCar1[] = { lateralDireita, -5, -1.8 };
	float dirCar1[] = { -1, 0.0, 0.0 };

	float posCar2[] = { -2.0, -5, -1.8 };
	float dirCar2[] = { -1, 0.0, 0.0 };


	objectsMobileLane7.push_back(new Car(posCar1, game, getLane5Velocity(), dirCar1));
	objectsMobileLane7.push_back(new Car(posCar2, game, getLane5Velocity(), dirCar2));

	//LANE 6

	float posCar3[] = { 2.0, -7.0, -1.8 };
	float dirCar3[] = { 1, 0.0, 0.0 };

	float posCar4[] = { lateralEsquerda, -7.0, -1.8 };
	float dirCar4[] = { 1, 0.0, 0.0 };

	objectsMobileLane8.push_back(new Car(posCar3, game, getLane6Velocity(), dirCar3));
	objectsMobileLane8.push_back(new Car(posCar4, game, getLane6Velocity(), dirCar4));


}

ManagerObj::~ManagerObj() {

}

void ManagerObj::draw() {
	for (Object* o : objects)
		o->draw();
	for (MobileObj* o : objectsMobileLane1)
		o->draw();
	for (MobileObj* o : objectsMobileLane2)
		o->draw();
	for (MobileObj* o : objectsMobileLane3)
		o->draw();
	for (MobileObj* o : objectsMobileLane6)
		o->draw();
	for (MobileObj* o : objectsMobileLane7)
		o->draw();
	for (MobileObj* o : objectsMobileLane8)
		o->draw();

	//bridge test
	for (MobileObj* o : objectsMobileLane4)
		o->draw();
	for (MobileObj* o : objectsMobileLane5)
		o->draw();


}

void ManagerObj::init(){


}

void ManagerObj::reset() {

}

void ManagerObj::update(float dt, Frog* frog) {

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	dificuldade = currentTime / 5000 + 1;


	//frog collision variables
	frogXXs = frog->getPositionXXs();
	frogYYs = frog->getPositionYYs();

	frogTopYYs = frogYYs + frogHeigth / 2;
	frogBottomYYs = frogYYs - frogHeigth / 2;

	canColideLane1 = false;
	canColideLane2 = false;
	canColideLane3 = false;
	canColideLane6 = false;
	canColideLane7 = false;
	canColideLane8 = false;

	canColideLane4 = false;
	canColideLane5 = false;

	onSurface = false;

	//YYS check
	//river collision lanes to use

	if ((11.8 > frogBottomYYs) && (10.2 < frogBottomYYs) || (11.8 > frogTopYYs) && (10.2 < frogTopYYs)){
		canColideLane1 = true;
	}
	if ((9.8 > frogBottomYYs) && (8.2 < frogBottomYYs) || (9.8 > frogTopYYs) && (8.2 < frogTopYYs)){
		canColideLane2 = true;
	}
	if ((7.8 > frogBottomYYs) && (6.2 < frogBottomYYs) || (7.8 > frogTopYYs) && (6.2 < frogTopYYs)){
		canColideLane3 = true;
	}
	if ((5.8 > frogBottomYYs) && (4.2 < frogBottomYYs) || (5.8 > frogTopYYs) && (4.2 < frogTopYYs)){
		canColideLane4 = true;
	}
	if ((3.8 > frogBottomYYs) && (2.2 < frogBottomYYs) || (3.8 > frogTopYYs) && (2.2 < frogTopYYs)){
		canColideLane5 = true;
	}

	//road collision lanes to use

	if ((-3.8 < frogBottomYYs) && (-2.2 > frogBottomYYs) || (-3.8 < frogTopYYs) && (-2.2 > frogTopYYs)){
		canColideLane6 = true;
	}
	if ((-5.8 < frogBottomYYs) && (-4.2 > frogBottomYYs) || (-5.8 < frogTopYYs) && (-4.2 > frogTopYYs)){
		canColideLane7 = true;
	}
	if ((-7.9 < frogBottomYYs) && (-6.3 > frogBottomYYs) || (-7.9 < frogTopYYs) && (-6.3 > frogTopYYs)){
		canColideLane8 = true;
	}



	//colision variables for the random //to be changed *******************************************************
	float position = 0.0;
	float newPosition = 0.0;
	float position2 = 0.0;
	float deltaPosition = 0.0;

	for (Object* o : objects){
		o->update(dt);
	}

	for (MobileObj* o : objectsMobileLane1){
		o->update(dt);

		//XXs check
		//soma das dimensões têm de ser maiores que a distância para haver colisão
		if (canColideLane1){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;

		}

		//random to be changed ****************************************************************
		o->multiplyVelocity(dificuldade);
		position = o->getPositionXXs();
		if (position > (lateralDireita - 4)){
			newPosition = lateralEsquerda + 4 - (rand() % 10);
			o->setPositionXXs(newPosition);
			for (MobileObj* o2 : objectsMobileLane1){
				position2 = o2->getPositionXXs();
				deltaPosition = (position - position2);
				if (0.0 < deltaPosition < 2.0)
					o->setPositionXXs(newPosition - 3 - deltaPosition / 2);
				if (-2.0 < deltaPosition < 0.0)
					o->setPositionXXs(newPosition - deltaPosition / 2);
			}
		}
	}
	for (MobileObj* o : objectsMobileLane2){
		o->update(dt);

		if (canColideLane2){
			if (((frogWeigth / 2 + turtleWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() < (lateralEsquerda - 4))
			o->setPositionXXs(lateralDireita + 4 + (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane3){
		o->update(dt);

		if (canColideLane3){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}

	for (MobileObj* o : objectsMobileLane4){
		o->update(dt);

		if (canColideLane4){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}

	for (MobileObj* o : objectsMobileLane5){
		o->update(dt);

		if (canColideLane5){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}

	for (MobileObj* o : objectsMobileLane6){
		o->update(dt);

		if (canColideLane6){
			if ((frogWeigth / 2 + busWeigth / 2) > fabs((frogXXs - o->getPositionXXs())))
				frog->killed();
		}

		o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane7){
		o->update(dt);

		if (canColideLane7){
			if ((frogWeigth / 2 + carWeigth / 2) > fabs((frogXXs - o->getPositionXXs())))
				frog->killed();
		}

		o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() < (lateralEsquerda - 4))
			o->setPositionXXs(lateralDireita + 4 + (rand() % 10));
	}
	for (MobileObj* o : objectsMobileLane8){
		o->update(dt);
		
		if (canColideLane8){
			if ((frogWeigth / 2 + carWeigth / 2) > fabs((frogXXs - o->getPositionXXs())))
				frog->killed();
		}

		o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4))
			o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
	}

	//se em river verificar se há alguma colisão, se houve então tudo bem.
	if ((frogYYs > 2.0) && (frogYYs < 12.0)){
		if (!onSurface)
			frog->killed();
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

	return 0 * dificuldade;
}
float ManagerObj::getLane2Velocity() {

	return 0 * dificuldade;
}
float ManagerObj::getLane3Velocity() {

	return 0 * dificuldade;
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