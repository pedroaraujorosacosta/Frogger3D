#include <iostream>
#include <string>

#include "ManagerLight.h"
#include "Light.h"
#include "Vector.h"

ManagerLight::ManagerLight(Game *game) : Manager(game) 
{
	init();
}

ManagerLight::~ManagerLight() {

}

void ManagerLight::illuminate() {
	for (Light *light : lights)
		light->illuminate();
}

void ManagerLight::draw() {

}

void ManagerLight::init()
{
	// point lights
	float dif[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector diff(dif, 4);
	float lpos[4] = { 0.0f, 0.0f, 14.0f, 1.0f };
	Vector lightPos(lpos, 4);
	Light *light1 = new Light(0, Light::POINT_LIGHT, game);
	light1->setPosition(lightPos);
	light1->setDiffuse(diff);
	light1->setAmbient(diff);
	light1->setSpecular(diff);

	lights.push_back(light1);

	// directional light
	float ldir[4] = { -1.0f, -1.0f, -1.0f, 0.0f };
	Vector lightDir(ldir, 4);
	Light *dirLight = new Light(1, Light::DIR_LIGHT, game);
	dirLight->setDiffuse(diff);
	dirLight->setAmbient(diff);
	dirLight->setSpecular(diff);
	dirLight->setPosition(lightDir);

	this->dirLight = dirLight;
	lights.push_back(dirLight);
}

void ManagerLight::reset() {

}

void ManagerLight::update() {

}

void ManagerLight::toggleDirectional()
{
	dirLight->toggleState();
}