#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__

#include <iostream>
#include <gl\glew.h>
#include <GL/freeglut.h>

class IComponent
{
public:
	virtual void draw() = 0;
	virtual void reset() = 0;
	virtual void update(float dt) = 0;
	virtual bool isOpenGLError();
	virtual void checkOpenGLError(std::string error);
};

#endif