#ifndef __TREE_H__
#define __TREE_H__

#include "BillboardObj.h"

class Tree : public BillboardObj {

protected:
	
	void init();
public:
	Tree(float* position, Game *game, float *direction, float width, float height);
};
#endif