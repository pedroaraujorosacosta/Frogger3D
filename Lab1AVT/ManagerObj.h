#ifndef __MANAGEROBJ_H__
#define __MANAGEROBJ_H__

#include "Manager.h"


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

class Object;
class MobileObj;

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

	float carWeigth;
	float busWeigth;
	float logWeigth;
	float turtleWeigth;

	//Optimization variables
	bool canColideLane1;
	bool canColideLane2;
	bool canColideLane3;
	bool canColideLane5;
	bool canColideLane6;
	bool canColideLane7;
	bool canColideLane8;
	bool canColideLane4;

	//Checking the river
	bool onSurface;

protected:

	void init();

public:
	ManagerObj(Game *game);
	~ManagerObj();
	void draw();
	void reset();
	void update(float dt,Frog* frog);

	void setDificuldade(float _dificuldade);
	void setLateralEsquerda(float _lateralEsquerda);
	void setLateralDireita(float _lateralDireita);

	float getLane1Velocity();
	float getLane2Velocity();
	float getLane3Velocity();
	float getLane4Velocity();
	float getLane5Velocity();
	float getLane6Velocity();
};

#endif