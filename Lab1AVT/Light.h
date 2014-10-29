#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "IComponent.h"

class Vector;
class Game;

class Light : public IComponent
{
public:
	enum LightType { POINT_LIGHT, DIR_LIGHT, SPOT_LIGHT };

private:
	LightType type;
	int numLight;
	Vector *amb;
	Vector *dif;
	Vector *spec;
	Vector *dir;
	Vector *pos;

	float cutOff;
	float exp;

	bool state;
	Game *game;

public:
	Light(int num, LightType type, Game *game);

	void update(float dt);
	void draw();
	void reset();
	void illuminate();
	void setDiffuse(Vector dif);
	void setAmbient(Vector amb);
	void setSpecular(Vector spec);
	void setPosition(Vector pos);
	void setDirection(Vector dir);
	void setCutoff(float cutOff);
	void setExponent(float exp);
	void toggleState();
	bool isPointLight();
};
#endif