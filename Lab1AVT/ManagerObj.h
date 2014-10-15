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
	std::vector<MobileObj*> objectsMobile;

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

	float getBusVelocity();
	float getCarVelocity();
	float getFloatingLogVelocity();
	float getTurtleVelocity();
};

#endif