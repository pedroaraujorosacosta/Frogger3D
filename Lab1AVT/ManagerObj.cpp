#include <iostream>
#include <string>
#include <assert.h>

#include "ManagerObj.h"
#include "Matrix.h"
#include "Game.h"

#include <gl\glew.h>
#include <GL\freeglut.h>

ManagerObj::ManagerObj(Game *game) : Manager(game) {
	this->game = game;
}

ManagerObj::~ManagerObj() {

}

void ManagerObj::draw(GLuint programID) {

}

void ManagerObj::init(){


}

void ManagerObj::reset() {

}

void ManagerObj::update() {

}