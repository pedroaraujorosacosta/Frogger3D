#include <iostream>
#include <string>
#include <assert.h>

#include "ManagerLight.h"
#include "Matrix.h"
#include "Game.h"

#include <gl\glew.h>
#include <GL\freeglut.h>

ManagerLight::ManagerLight(Game *game) : Manager(game) {
	this->game = game;
}

ManagerLight::~ManagerLight() {

}

void ManagerLight::draw(GLuint programID) {

}

void ManagerLight::init(){

}

void ManagerLight::reset() {

}

void ManagerLight::update() {

}