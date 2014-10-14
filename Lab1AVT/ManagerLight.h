#ifndef __MANAGERLIGHT_H__
#define __MANAGERLIGHT_H__

#include "Manager.h"
#include <GL\freeglut.h>

#define MY_VERTEX_COORD_ATTRIB 0
#define MY_NORMAL_ATTRIB 1
#define MY_TEXTURE_COORD_ATTRIB 2

class Game;

class ManagerLight : public Manager
{

protected:

	virtual void init();

	void sendDataToShader(GLuint programID);
	void createBufferObjects();
public:
	ManagerLight(Game *game);
	~ManagerLight();
	virtual void draw(GLuint programID);
	virtual void reset();
	virtual void update();
};

#endif