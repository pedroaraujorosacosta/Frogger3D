#ifndef __RIVER_H__
#define __RIVER_H__

#include "Object.h"
class Cube;
class Tree;
class River : public Object {

	Cube *bankTop;
	Cube *river;
	Cube *bankBot;
	Tree *tree;

protected:
	void init();
public:
	River(float* position, Game *game);
	~River();
	void draw();
};
#endif