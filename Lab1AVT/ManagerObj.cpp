#include <iostream>
#include <string>
#include <algorithm>

#include "Object.h"
#include "MobileObj.h"
#include "Game.h"
#include "Frog.h"

#include "River.h"
#include "Road.h"
#include "Turtle.h"
#include "FloatingLog.h"
#include "Bus.h"
#include "Car.h"
#include "Tree.h"
#include "ManagerObj.h"


ManagerObj::ManagerObj(Game *game) : Manager(game) {

	setDificuldade(1);
	setLateralEsquerda(-20.0);
	setLateralDireita(20.0);

	//stencil
	float positionCube[]{0.0, 1.0, -5.0};
	this->stencilMaskCube = new Cube(positionCube, game, false,0);

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

	// Trees
	// init trees
	float dir[3] = { 0.0, -1.0, 0.0 };
	float tree1f[3] = { 0.0f, 1.0f, -2.0f };
	Tree *tree1 = new Tree(tree1f, game, dir, 1.0, 1.0);
	objects.push_back(tree1);

	float tree2f[3] = { 8.0f, 1.0f, -2.0f };
	Tree *tree2 = new Tree(tree2f, game, dir, 1.0, 1.0);
	objects.push_back(tree2);

	float tree3f[3] = { -8.0f, 1.0f, -2.0f };
	Tree *tree3 = new Tree(tree3f, game, dir, 1.0, 1.0);
	objects.push_back(tree3);

	float tree4f[3] = { 0.0f, 13.0f, -2.0f };
	Tree *tree4 = new Tree(tree4f, game, dir, 1.0, 1.0);
	objects.push_back(tree4);

	float tree5f[3] = { 8.0f, 13.0f, -2.0f };
	Tree *tree5 = new Tree(tree5f, game, dir, 1.0, 1.0);
	objects.push_back(tree5);

	float tree6f[3] = { -8.0f, 13.0f, -2.0f };
	Tree *tree6 = new Tree(tree6f, game, dir, 1.0, 1.0);
	objects.push_back(tree6);

	//LANE 1
	float posFloatingLog1[] = { -5.0, 11.0, -2.5 };
	float dirFloatingLog1[] = { 1, 0.0, 0.0 };

	float posFloatingLog2[] = { 1.0, 11.0, -2.5 };
	float dirFloatingLog2[] = { 1, 0.0, 0.0 };

	float posFloatingLog3[] = { 8.0, 11.0, -2.5 };
	float dirFloatingLog3[] = { 1, 0.0, 0.0 };

	FloatingLog *fLog1 = new FloatingLog(posFloatingLog1, game, getLane1Velocity(), dirFloatingLog1);
	FloatingLog *fLog2 = new FloatingLog(posFloatingLog2, game, getLane1Velocity(), dirFloatingLog2);
	FloatingLog *fLog3 = new FloatingLog(posFloatingLog3, game, getLane1Velocity(), dirFloatingLog3);
	objectsMobileLane1.push_back(fLog1);
	objectsMobileLane1.push_back(fLog2);
	objectsMobileLane1.push_back(fLog3);
	objects.push_back(fLog1);
	objects.push_back(fLog2);
	objects.push_back(fLog3);

	//LANE 2

	float posTurtle1[] = { lateralDireita, 9, -2.5 };
	float dirTurtle1[] = { -1, 0.0, 0.0 };

	float posTurtle2[] = { 2.0, 9, -2.5 };
	float dirTurtle2[] = { -1, 0.0, 0.0 };

	float posTurtle3[] = { -4, 9, -2.5 };
	float dirTurtle3[] = { -1, 0.0, 0.0 };
	
	Turtle *turtle1 = new Turtle(posTurtle1, game, getLane2Velocity(), dirTurtle1);
	Turtle *turtle2 = new Turtle(posTurtle2, game, getLane2Velocity(), dirTurtle2);
	Turtle *turtle3 = new Turtle(posTurtle3, game, getLane2Velocity(), dirTurtle3);
	objectsMobileLane2.push_back(turtle1);
	objectsMobileLane2.push_back(turtle2);
	objectsMobileLane2.push_back(turtle3);
	objects.push_back(turtle1);
	objects.push_back(turtle2);
	objects.push_back(turtle3);

	//LANE 3

	float posFloatingLog4[] = { lateralEsquerda, 7.0, -2.5 };
	float dirFloatingLog4[] = { 1, 0.0, 0.0 };

	float posFloatingLog5[] = { 7.0, 7.0, -2.5 };
	float dirFloatingLog5[] = { 1, 0.0, 0.0 };

	float posFloatingLog6[] = { -2.0, 7.0, -2.5 };
	float dirFloatingLog6[] = { 1, 0.0, 0.0 };

	FloatingLog *fLog4 = new FloatingLog(posFloatingLog4, game, getLane3Velocity(), dirFloatingLog4);
	FloatingLog *fLog5 = new FloatingLog(posFloatingLog5, game, getLane3Velocity(), dirFloatingLog5);
	FloatingLog *fLog6 = new FloatingLog(posFloatingLog6, game, getLane3Velocity(), dirFloatingLog6);
	objectsMobileLane3.push_back(fLog4);
	objectsMobileLane3.push_back(fLog5);
	objectsMobileLane3.push_back(fLog6);
	objects.push_back(fLog4);
	objects.push_back(fLog5);
	objects.push_back(fLog6);

	//BRIDGE new Lane 4

	float posFloatingLog7[] = { 0.0, 5.0, -2.5 };
	float dirFloatingLog7[] = { 1, 0.0, 0.0 };

	FloatingLog *fLog7 = new FloatingLog(posFloatingLog7, game, getLane4Velocity(), dirFloatingLog4);
	objectsMobileLane4.push_back(fLog7);
	objects.push_back(fLog7);

	//BRIDGE new Lane 5

	float posFloatingLog8[] = { 0.0, 3.0, -2.5 };
	float dirFloatingLog8[] = { 1, 0.0, 0.0 };

	FloatingLog *fLog8 = new FloatingLog(posFloatingLog8, game, getLane5Velocity(), dirFloatingLog4);
	objectsMobileLane5.push_back(fLog8);
	objects.push_back(fLog8);

	//LANE 4

	float posBus[] = { lateralEsquerda, -3, -1.8 };
	float dirBus[] = { 1, 0.0, 0.0 };

	Bus *bus1 = new Bus(posBus, game, getLane6Velocity(), dirBus);
	objectsMobileLane6.push_back(bus1);
	objects.push_back(bus1);

	//LANE 5

	float posCar1[] = { lateralDireita, -5, -1.8 };
	float dirCar1[] = { -1, 0.0, 0.0 };

	float posCar2[] = { -2.0, -5, -1.8 };
	float dirCar2[] = { -1, 0.0, 0.0 };

	Car *car1 = new Car(posCar1, game, getLane7Velocity(), dirCar1);
	Car *car2 = new Car(posCar2, game, getLane7Velocity(), dirCar2);
	objectsMobileLane7.push_back(car1);
	objectsMobileLane7.push_back(car2);
	objects.push_back(car1);
	objects.push_back(car2);

	//LANE 6

	float posCar3[] = { 2.0, -7.0, -1.8 };
	float dirCar3[] = { 1, 0.0, 0.0 };

	float posCar4[] = { lateralEsquerda, -7.0, -1.8 };
	float dirCar4[] = { 1, 0.0, 0.0 };

	Car *car3 = new Car(posCar3, game, getLane8Velocity(), dirCar3);
	Car *car4 = new Car(posCar4, game, getLane8Velocity(), dirCar4);
	objectsMobileLane8.push_back(car3);
	objectsMobileLane8.push_back(car4);
	objects.push_back(car3);
	objects.push_back(car4);
}

ManagerObj::~ManagerObj() {
	for (Object* o : objects)
		delete o;
}

void ManagerObj::draw() {
	for (std::vector<Object*>::reverse_iterator it = objects.rbegin(); it != objects.rend(); it++)
		(*it)->draw();
}

void ManagerObj::init(){

}

void ManagerObj::reset() {

	dificuldade = 1;

	for (MobileObj* o : objectsMobileLane1){
		o->reset();
	}
	for (MobileObj* o : objectsMobileLane2){
		o->reset();
	}
	for (MobileObj* o : objectsMobileLane3){
		o->reset();
	}
	for (MobileObj* o : objectsMobileLane4){
		o->reset();
	}
	for (MobileObj* o : objectsMobileLane5){
		o->reset();
	}
	for (MobileObj* o : objectsMobileLane6){
		o->reset();
	}
	for (MobileObj* o : objectsMobileLane7){
		o->reset();
	}
	for (MobileObj* o : objectsMobileLane8){
		o->reset();
	}
}

bool ManagerObj::laneCollision(std::vector<MobileObj*> lane, MobileObj* obj)
{
	bool collision = false;
	for (MobileObj *el : lane) 
	{
		if (el != obj)
		{
			if ((obj->getWidth() / 2 + el->getWidth() / 2) > fabs((obj->getPositionXXs() - el->getPositionXXs())))
				collision = true;
		}
	}

	return collision;
}

float ManagerObj::getRiverLaneVelocity(Vector &laneDir)
{
	canColideLane1 = false;
	canColideLane2 = false;
	canColideLane3 = false;
	canColideLane4 = false;
	canColideLane5 = false;
	laneDir = Vector(3);

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

	for (MobileObj* o : objectsMobileLane1)
	{
		//XXs check
		//soma das dimens�es t�m de ser maiores que a dist�ncia para haver colis�o
		if (canColideLane1){
			if (((frogWeigth / 2 + o->getWidth() / 2) > fabs((frogXXs - o->getPositionXXs()))))
			{
				laneDir = getLane1Direction();
				return o->getVelocity();
			}
		}
	}

	for (MobileObj* o : objectsMobileLane2)
	{
		//XXs check
		//soma das dimens�es t�m de ser maiores que a dist�ncia para haver colis�o
		if (canColideLane2){
			if (((frogWeigth / 2 + o->getWidth() / 2) > fabs((frogXXs - o->getPositionXXs()))))
			{
				laneDir = getLane2Direction();
				return o->getVelocity();
			}
		}
	}

	for (MobileObj* o : objectsMobileLane3)
	{
		//XXs check
		//soma das dimens�es t�m de ser maiores que a dist�ncia para haver colis�o
		if (canColideLane3){
			if (((frogWeigth / 2 + o->getWidth() / 2) > fabs((frogXXs - o->getPositionXXs()))))
			{
				laneDir = getLane3Direction();
				return o->getVelocity();
			}
		}
	}

	for (MobileObj* o : objectsMobileLane4)
	{
		//XXs check
		//soma das dimens�es t�m de ser maiores que a dist�ncia para haver colis�o
		if (canColideLane4){
			if (((frogWeigth / 2 + o->getWidth() / 2) > fabs((frogXXs - o->getPositionXXs()))))
			{
				laneDir = getLane4Direction();
				return o->getVelocity();
			}
		}
	}

	for (MobileObj* o : objectsMobileLane5)
	{
		//XXs check
		//soma das dimens�es t�m de ser maiores que a dist�ncia para haver colis�o
		if (canColideLane5){
			if (((frogWeigth / 2 + o->getWidth() / 2) > fabs((frogXXs - o->getPositionXXs()))))
			{
				laneDir = getLane5Direction();
				return o->getVelocity();
			}
		}
	}

	return 0.0f;
}

void ManagerObj::update(float dt, Frog* frog) {

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	dificuldade = (currentTime - game->getStartTime()) / 5000 + 1;
	if (dificuldade > 15) dificuldade = 15;

	//frog collision variables
	frogXXs = frog->getPositionXXs();
	frogYYs = frog->getPositionYYs();

	frogTopYYs = frogYYs + frogHeigth / 2;
	frogBottomYYs = frogYYs - frogHeigth / 2;
	frogRigthXXs = frogXXs + frogWeigth / 2;
	frogLeftXXs = frogYYs - frogWeigth / 2;

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

	// sort objects (so translucids appear ordered according to position relative to camera)
	std::sort(objects.begin(), objects.end(), comparatorObjects);


	for (Object* o : objects){
		o->update(dt);
	}
	for (MobileObj* o : objectsMobileLane1){

		//XXs check
		//soma das dimens�es t�m de ser maiores que a dist�ncia para haver colis�o
		if (canColideLane1){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;

		}

		if(dificuldade != 15)
			o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4)){
			do {
				o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
			} while (laneCollision(objectsMobileLane1, o));
		}

	}
	for (MobileObj* o : objectsMobileLane2){
		if (canColideLane2){
			if (((frogWeigth / 2 + turtleWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		if (dificuldade != 15)
			o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() < (lateralEsquerda - 4)){
			do {
				o->setPositionXXs(lateralDireita + 4 + (rand() % 10));
			} while (laneCollision(objectsMobileLane2, o));
		}

	}
	for (MobileObj* o : objectsMobileLane3){

		if (canColideLane3){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		if (dificuldade != 15)
			o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4)){
			do {
				o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
			} while (laneCollision(objectsMobileLane3, o));
		}
	}

	for (MobileObj* o : objectsMobileLane4){

		if (canColideLane4){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		if (dificuldade != 15)
			o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4)){
			do {
				o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
			} while (laneCollision(objectsMobileLane4, o));
		}
	}

	for (MobileObj* o : objectsMobileLane5){

		if (canColideLane5){
			if (((frogWeigth / 2 + logWeigth / 2) > fabs((frogXXs - o->getPositionXXs()))))
				onSurface = true;
		}

		if (dificuldade != 15)
			o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4)){
			do {
				o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
			} while (laneCollision(objectsMobileLane5, o));
		}

	}

	for (MobileObj* o : objectsMobileLane6){

		if (canColideLane6){
			if ((frogWeigth / 2 + busWeigth / 2) > fabs((frogXXs - o->getPositionXXs())))
				frog->killed();
		}

		if (dificuldade != 15)
			o->multiplyVelocity(dificuldade);
		if (o->getPositionXXs() > (lateralDireita + 4)){
			do {
				o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
			} while (laneCollision(objectsMobileLane6, o));
		}

	}
	for (MobileObj* o : objectsMobileLane7){
		

		if (canColideLane7){
			if ((frogWeigth / 2 + carWeigth / 2) > fabs((frogXXs - o->getPositionXXs())))
				frog->killed();
		}

		if (dificuldade != 15)
			o->multiplyVelocity(dificuldade);

		if (o->getPositionXXs() < (lateralEsquerda - 4)){
			do {
				o->setPositionXXs(lateralDireita + 4 + (rand() % 10));
			} while (laneCollision(objectsMobileLane7, o));
		}


	}
	for (MobileObj* o : objectsMobileLane8){
	
		
		if (canColideLane8){
			if ((frogWeigth / 2 + carWeigth / 2) > fabs((frogXXs - o->getPositionXXs())))
				frog->killed();
		}

		if (dificuldade != 15)
			o->multiplyVelocity(dificuldade);

		if (o->getPositionXXs() > (lateralDireita + 4)){
			do {
				o->setPositionXXs(lateralEsquerda - 4 - (rand() % 10));
			} while (laneCollision(objectsMobileLane8, o));
		}
	
	}

	float vel[3];

	
	//se em river verificar se h� alguma colis�o, se houve ent�o tudo bem.
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

	return 0.2 * dificuldade;
}
float ManagerObj::getLane2Velocity() {

	return 0.8 * dificuldade;
}
float ManagerObj::getLane3Velocity() {

	return 0.2 * dificuldade;
}
float ManagerObj::getLane4Velocity() {

	return 0 * dificuldade;
}
float ManagerObj::getLane5Velocity() {

	return 0 * dificuldade;
}
float ManagerObj::getLane6Velocity() {

	return 1 * dificuldade;
}
float ManagerObj::getLane7Velocity() {

	return 2 * dificuldade;
}
float ManagerObj::getLane8Velocity() {

	return 2 * dificuldade;
}

Vector ManagerObj::getLane1Direction()
{
	float dirLane1[] = { 1, 0.0, 0.0 };
	Vector dir(dirLane1, 3);
	return dir;
}

Vector ManagerObj::getLane2Direction()
{
	float dirLane2[] = { -1, 0.0, 0.0 };
	Vector dir(dirLane2, 3);
	return dir;
}

Vector ManagerObj::getLane3Direction()
{
	float dirLane3[] = { 1, 0.0, 0.0 };
	Vector dir(dirLane3, 3);
	return dir;
}

Vector ManagerObj::getLane4Direction()
{
	float dirLane4[] = { 1, 0.0, 0.0 };
	Vector dir(dirLane4, 3);
	return dir;
}

Vector ManagerObj::getLane5Direction()
{
	float dirLane5[] = { 1, 0.0, 0.0 };
	Vector dir(dirLane5, 3);
	return dir;
}


void ManagerObj::applyStencil(){


	glClearStencil(0x0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	


	stencilMaskCube->setScale(2, 2, 2);
	stencilMaskCube->draw();

	
	glClear(GL_DEPTH_BUFFER_BIT);
	glStencilFunc(GL_EQUAL,0x0, 0x1); //se 0x0 passar a 0x1, o stencil � invertido e imagem fica no cubo
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	
}