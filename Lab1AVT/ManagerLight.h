#ifndef __MANAGERLIGHT_H__
#define __MANAGERLIGHT_H__

#include "Manager.h"

class Game;

class ManagerLight : public Manager
{

protected:

	virtual void init();

public:
	ManagerLight(Game *game);
	~ManagerLight();
	virtual void draw();
	virtual void reset();
	virtual void update();
};

#endif