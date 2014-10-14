#ifndef __TURTLE_H__
#define __TURTLE_H__

#include "Surface.h"
#include "Cube.h"
#include "Sphere.h"

class Turtle : public Surface {

	Cube *body;
	Sphere *wheels;


protected:
	void init();
public:
	Turtle(float* position, Game *game, float velocity, float *direction);
	~Turtle();
	void draw(GLuint programID);
	void update();
};
#endif