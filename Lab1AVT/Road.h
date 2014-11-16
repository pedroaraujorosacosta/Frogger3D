#ifndef __ROAD_H__
#define __ROAD_H__

#include "Object.h"
#include "Cube.h"
#include "Sphere.h"
#include "Monkey.h"

class Road : public Object {

	Cube *side;
	Cube *road;
	Cube *lampPost;
	Sphere *lampadaTopo;
	Monkey *obj;
	
protected:
	void init();
public:
	Road(float* position, Game *game);
	~Road();
	void draw();
};
#endif