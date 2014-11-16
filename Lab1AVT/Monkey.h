#ifndef __MONKEY_H__
#define __MONKEY_H__

#include "Object.h"

class Monkey : public Object {
protected:
	void init();
public:
	Monkey(float* position, Game *game, bool isRepeatTexture = false, float factorX = 1.0f);
	void draw();
};
#endif