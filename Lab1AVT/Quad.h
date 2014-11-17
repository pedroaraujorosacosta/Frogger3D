#ifndef __QUAD_H__
#define __QUAD_H__

#include "Object.h"

class Quad : public Object {
protected:
	void init();
	void initCustomQuad(float originX, float originY, float width, float height);
public:
	Quad(float* position, Game *game);
	Quad(float *position, float originX, float originY, float width, float height, Game *game);

	void draw();
	void drawCustomQuad(float originX, float originY, float width, float height);
};
#endif