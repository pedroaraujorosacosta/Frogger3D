#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Object.h"

class Quad;

class Skybox : public Object {
protected:
	Quad *quad;

	void init();
public:
	Skybox(float* position, Game *game);
	~Skybox();

	void draw();
};
#endif