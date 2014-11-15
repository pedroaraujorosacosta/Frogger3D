#include <iostream>
#include <string>

#include "ManagerLight.h"
#include "Light.h"
#include "Vector.h"
#include "Game.h"

ManagerLight::ManagerLight(Game *game) : Manager(game)
{
	init();
}

ManagerLight::~ManagerLight() {
	for (Light *light : lights)
		delete light;
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
	float dif[4] = { 0.7f, 0.7f, 0.3f, 1.0f };
	Vector diff(dif, 4);
	float specPointf[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	Vector specPoint(specPointf, 4);
	float lpos1[4] = { 0.0f, -1.0f, 0.5f, 1.0f };
	Vector lightPos1(lpos1, 4);
	Light *light1 = new Light(0, Light::POINT_LIGHT, game);
	light1->setPosition(lightPos1);
	light1->setDiffuse(diff);
	light1->setAmbient(diff);
	light1->setSpecular(specPoint);
	lights.push_back(light1);

	float lpos2[4] = { 10.0f, -1.0f, 0.5f, 1.0f };
	Vector lightPos2(lpos2, 4);
	Light *light2 = new Light(1, Light::POINT_LIGHT, game);
	light2->setPosition(lightPos2);
	light2->setDiffuse(diff);
	light2->setAmbient(diff);
	light2->setSpecular(specPoint);
	lights.push_back(light2);

	float lpos3[4] = { -10.0f, -1.0f, 0.5f, 1.0f };
	Vector lightPos3(lpos3, 4);
	Light *light3 = new Light(2, Light::POINT_LIGHT, game);
	light3->setPosition(lightPos3);
	light3->setDiffuse(diff);
	light3->setAmbient(diff);
	light3->setSpecular(specPoint);
	lights.push_back(light3);

	float lpos4[4] = { 0.0f, -13.0f, 0.5f, 1.0f };
	Vector lightPos4(lpos4, 4);
	Light *light4 = new Light(3, Light::POINT_LIGHT, game);
	light4->setPosition(lightPos4);
	light4->setDiffuse(diff);
	light4->setAmbient(diff);
	light4->setSpecular(specPoint);
	lights.push_back(light4);

	float lpos5[4] = { 10.0f, -13.0f, 0.5f, 1.0f };
	Vector lightPos5(lpos5, 4);
	Light *light5 = new Light(4, Light::POINT_LIGHT, game);
	light5->setPosition(lightPos5);
	light5->setDiffuse(diff);
	light5->setAmbient(diff);
	light5->setSpecular(specPoint);
	lights.push_back(light5);

	float lpos6[4] = { -10.0f, -13.0f, 0.5f, 1.0f };
	Vector lightPos6(lpos6, 4);
	Light *light6 = new Light(5, Light::POINT_LIGHT, game);
	light6->setPosition(lightPos6);
	light6->setDiffuse(diff);
	light6->setAmbient(diff);
	light6->setSpecular(specPoint);
	lights.push_back(light6);

	// directional light
	float difDirf[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector diffDir(difDirf, 4);
	float ldir[4] = { -1.0f, -1.0f, -1.0f, 0.0f };
	Vector lightDir(ldir, 4);
	Light *dirLight = new Light(6, Light::DIR_LIGHT, game);
	dirLight->setDiffuse(diffDir);
	dirLight->setAmbient(diffDir);
	dirLight->setSpecular(diffDir);
	dirLight->setPosition(lightDir);
	this->dirLight = dirLight;
	lights.push_back(dirLight);

	// spot light
	float difSpotf[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector difSpot(difSpotf, 4);
	float lSpotPosf[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	Vector lSpotPos(lSpotPosf, 4);
	float lSpotDirf[3] = { 0.0f, 0.0f, -1.0f };
	Vector lSpotDir(lSpotDirf, 3);
	Light *spot = new Light(7, Light::SPOT_LIGHT, game);
	spot->setDiffuse(difSpot);
	spot->setAmbient(difSpot);
	spot->setSpecular(difSpot);
	spot->setPosition(lSpotPos);
	spot->setDirection(lSpotDir);
	spot->setCutoff(0.5f);
	spot->setExponent(0.1f);
	spotLight = spot;
	lights.push_back(spot);
}

void ManagerLight::togglePointLights()
{
	for (Light *l : lights)
	{
		if (l->isPointLight())
			l->toggleState();
	}
}

void ManagerLight::reset() {

}

void ManagerLight::update() {

}

void ManagerLight::toggleDirectional()
{
	dirLight->toggleState();
}

void ManagerLight::toggleSpotLight()
{
	spotLight->toggleState();
}

Light* ManagerLight::getSpotLight()
{
	return spotLight;
}