#include <iostream>
#include <string>
#include <assert.h>

#include "Manager.h"
#include "Matrix.h"
#include "Game.h"

#include <gl\glew.h>
#include <GL\freeglut.h>

Manager::Manager(Game *game) {
	this->game = game;
}

Manager::~Manager() {

}

void Manager::draw(GLuint programID) {

}

void Manager::init(){


}

void Manager::reset() {

}

void Manager::update() {

}