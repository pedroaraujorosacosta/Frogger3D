#ifndef __RIVER_H__
#define __RIVER_H__

#include "Object.h"

class River : public Object {

	Cube *bankTop;
	Cube *river;
	Cube *bankBot;

protected:
	void init();
public:
	River(float* position, Game *game);
	~River();
	void draw(GLuint programID);
};
#endif