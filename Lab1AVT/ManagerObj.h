#ifndef __MANAGEROBJ_H__
#define __MANAGEROBJ_H__

#include "Manager.h"
#include "Vector.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

class Object;
class MobileObj;
class Particle;
class ParticleSystem;

class ManagerObj : public Manager
{
	float dificuldade;
	float lateralEsquerda;
	float lateralDireita;

	std::vector<Object*> objects;
	std::vector<MobileObj*> objectsMobileLane1;
	std::vector<MobileObj*> objectsMobileLane2;
	std::vector<MobileObj*> objectsMobileLane3;
	std::vector<MobileObj*> objectsMobileLane4;
	std::vector<MobileObj*> objectsMobileLane5;
	std::vector<MobileObj*> objectsMobileLane6;
	std::vector<MobileObj*> objectsMobileLane7;
	std::vector<MobileObj*> objectsMobileLane8;

	//frog collision variables
	float frogXXs;
	float frogYYs;

	float frogHeigth;
	float frogWeigth;

	float frogTopYYs;
	float frogBottomYYs;
	float frogRigthXXs;
	float frogLeftXXs;

	float carWeigth;
	float busWeigth;
	float logWeigth;
	float turtleWeigth;

	//para o random start
	int randomIndex;

	//Optimization variables
	bool canColideLane1;
	bool canColideLane2;
	bool canColideLane3;
	bool canColideLane5;
	bool canColideLane6;
	bool canColideLane7;
	bool canColideLane8;
	bool canColideLane4;

	//vector de vector
	bool** validPositions;

	//Checking the river
	bool onSurface;

	Particle* p;
	ParticleSystem *ps;

protected:

	void init();

public:
	ManagerObj(Game *game);
	~ManagerObj();
	void draw();
	void reset();
	void update(float dt, Frog* frog);

	void setDificuldade(float _dificuldade);
	void setLateralEsquerda(float _lateralEsquerda);
	void setLateralDireita(float _lateralDireita);

	float ositionGenerator();
	float validateRandomPosition();

	float getLane1Velocity();
	float getLane2Velocity();
	float getLane3Velocity();
	float getLane4Velocity();
	float getLane5Velocity();
	float getLane6Velocity();
	float getLane7Velocity();
	float getLane8Velocity();

	Vector getLane1Direction();
	Vector getLane2Direction();
	Vector getLane3Direction();
	Vector getLane4Direction();
	Vector getLane5Direction();

	int validateRandomPosition(int lane, int randomIndex);
	int getRandomStartPosition(int randomIndex);
	void updateValidPositions(int lane, float objectXXs);

	bool laneCollision(std::vector<MobileObj*> lane, MobileObj* obj);
	float getRiverLaneVelocity(Vector &laneDir);
};

#endif