#ifndef __MANAGEROBJ_H__
#define __MANAGEROBJ_H__

#include "Manager.h"
#include <GL\freeglut.h>

#define MY_VERTEX_COORD_ATTRIB 0
#define MY_NORMAL_ATTRIB 1
#define MY_TEXTURE_COORD_ATTRIB 2

class Game;

class ManagerObj : public Manager
{

protected:

	virtual void init();

	void sendDataToShader(GLuint programID);
	void createBufferObjects();
public:
	ManagerObj(Game *game);
	~ManagerObj();
	virtual void draw(GLuint programID);
	virtual void reset();
	virtual void update();
};

#endif