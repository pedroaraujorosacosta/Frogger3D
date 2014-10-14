#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "IComponent.h"
#include <GL\freeglut.h>

#define MY_VERTEX_COORD_ATTRIB 0
#define MY_NORMAL_ATTRIB 1
#define MY_TEXTURE_COORD_ATTRIB 2

class Game;

class Manager : public IComponent
{

protected:

	bool active;
	Game *game;


	virtual void init();

	void sendDataToShader(GLuint programID);
	void createBufferObjects();
public:
	Manager(Game *game);
	~Manager();
	virtual void draw(GLuint programID);
	virtual void reset();
	virtual void update();
};

#endif