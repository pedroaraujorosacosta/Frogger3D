#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "IComponent.h"
#include <GL\freeglut.h>

#define MY_VERTEX_COORD_ATTRIB 0
#define MY_NORMAL_ATTRIB 1
#define MY_TEXTURE_COORD_ATTRIB 2

struct Material{
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texCount;
};

class Game;

class Object : public IComponent
{

protected:
	float position[3];
	bool active;
	float *vertices;
	float *normals;
	float *texCoords;
	unsigned int *faceIndex;
	int verticeCount;
	int faceCount;
	GLuint VaoId;
	GLuint VboId[4];
	Game *game;
	struct Material mat;

	virtual void init();

	void sendMaterials();
	void sendDataToShader();
	void createBufferObjects();
public:
	Object(float* position, Game *game);
	~Object();
	virtual void draw();
	virtual void reset();
	virtual void update(float dt); 

	float * circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX = 0.0f, float transY = 0.0f);
	int revSmoothNormal2(float *p, float *nx, float *ny, float smoothCos, int beginEnd);
	void computeVAO(int numP, float *p, float *points, int sides, float smoothCos);

	void setAmbient(float *amb);
	void setDiffuse(float *diff);
	void setSpecular(float *spec);
	void setEmissive(float *emissive);
	void setShininess(float shininess);
	void setTexCount(int texCount);

	float getPositionXXs(){
		return position[0];
	}
	void setPositionXXs(float positionXXs){
		position[0] = positionXXs;
	}
	float getPositionYYs(){
		return position[1];
	}
	void setPositionYYs(float positionYYs){
		position[1] = positionYYs;
	}
	float getPositionZZs(){
		return position[2];
	}
	void setPositionZZs(float positionZZs){
		position[2] = positionZZs;
	}
};

#endif