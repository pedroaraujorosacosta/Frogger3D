#ifndef __MANAGERLIGHT_H__
#define __MANAGERLIGHT_H__

#include "Manager.h"
#include <vector>

class Game;
class Light;

class ManagerLight : public Manager
{
protected:
	std::vector<Light*> lights;

	virtual void init();

public:
	ManagerLight(Game *game);
	~ManagerLight();
	void illuminate();
	virtual void draw();
	virtual void reset();
	virtual void update();
};

#endif