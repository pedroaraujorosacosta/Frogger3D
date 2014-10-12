#ifndef __IOBJECT_H__
#define __IOBJECT_H__

#include <iostream>
#include <gl\glew.h>
#include <GL/freeglut.h>

class IComponent
{
public:
	virtual void draw(GLuint programID) = 0;
	virtual void reset() = 0;
	virtual void update() = 0;
	virtual bool isOpenGLError();
	virtual void checkOpenGLError(std::string error);
};

#endif