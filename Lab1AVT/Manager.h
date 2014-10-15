#ifndef __MANAGER_H__
#define __MANAGER_H__


class Game;

class Manager
{


protected:

	bool active;
	Game *game;


	virtual void init();

public:
	Manager(Game *game);
	~Manager();
	virtual void draw();
	virtual void reset();
	virtual void update();
};

#endif