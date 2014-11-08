#include "Tree.h"
#include "Stack.h"
#include "Game.h"

Tree::Tree(float *position, Game *game, float *direction, float width, float height) : BillboardObj(position, game, direction, width, height)
{
	for (int i = 0; i < 3; i++)
		this->direction[i] = direction[i];

	this->width = width;
	this->height = height;

	init();
	createBufferObjects();
}

void Tree::init() {
	setTexCount(4);
}
