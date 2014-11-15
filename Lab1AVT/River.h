#ifndef __RIVER_H__
#define __RIVER_H__

#include <vector>
#include "Object.h"

class Cube;
class Tree;
class River : public Object {
	
	Cube *riverWater;
	std::vector<Object*> objects;

protected:
	void init();
public:
	River(float* position, Game *game);
	~River();
	void draw();
	void update(float dt);
};
#endif