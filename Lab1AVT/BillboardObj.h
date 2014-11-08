#ifndef __BILLBOARDOBJ_H__
#define __BILLBOARDOBJ_H__

#include "Object.h"
#include "Quad.h"

class BillboardObj : public Object {

protected:
	Quad *quad;

	float direction[3];
	float width;
	float height;

	virtual void init();
public:
	BillboardObj(float* position, Game *game, float *direction, float width, float height);
	~BillboardObj();
	virtual void draw();
	float getDirX();
	float getDirY();
	float getDirZ();
	float getWidth();
};
#endif