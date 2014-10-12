#ifndef __ROAD_H__
#define __ROAD_H__

#include "Object.h"

class Road : public Object {

	Cube *sideTop;
	Cube *road;
	Cube *sideBot;

protected:
	void init();
public:
	Road(float* position, Game *game);
	~Road();
	void draw(GLuint programID);
};
#endif