#ifndef __CUBE_H__
#define __CUBE_H__

#include "Object.h"

class Cube : public Object {
protected:
	bool isRepeatTexture;
	float repeatFactorX;

	void init();
public:
	Cube(float* position, Game *game, bool isRepeatTexture = false, float factorX = 1.0f);
	void draw();
};
#endif