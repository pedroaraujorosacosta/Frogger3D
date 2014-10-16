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

protected:

	void init();

public:
	ManagerObj(Game *game);
	~ManagerObj();
	void draw();
	void reset();
	void update();

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