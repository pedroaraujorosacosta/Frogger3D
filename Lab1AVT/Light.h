#ifndef __LIGHT_H__
#define __LIGHT_H__


class Vector;

class Light
{
	int numLight;
	Vector *amb;
	Vector *dif;
	Vector *spec;

	Vector *pos;
	Vector *dir;

	float cutOff;
	float exp;

	bool state;

	
public:
	Light(int num);

	void setDiffuse(Vector *dif);
	void setAmbient(Vector *amb);
	void setSpecular(Vector *spec);
	void setPosition(Vector *pos);
	void setDirection(Vector *dir);
	void setCutoff(float cutOff);
	void setExponent(float exp);
};
#endif