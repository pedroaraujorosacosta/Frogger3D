#ifndef __LIGHT_H__
#define __LIGHT_H__


#include "Vector.h"

class Light
{
	int numLight;
	Vector amb(4);
	Vector dif(4);
	Vector spec(4);

	Vector pos(4);
	Vector dir(3);

	float cutOff;
	float exp;

	bool state;

	
public:
	Light(int num);

	void setDiffuse(Vector dif);
	void setAmbient(Vector amb);
	void setSpecular(Vector spec);
	void setPosition(Vector pos);
	void setDirection(Vector dir);
	void setCutoff(float cutOff);
	void setExponent(float exp);
};
#endif