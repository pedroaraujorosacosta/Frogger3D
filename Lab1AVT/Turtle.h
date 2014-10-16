#ifndef __TURTLE_H__
#define __TURTLE_H__

#include "Surface.h"
#include "Torus.h"
#include "Sphere.h"

class Turtle : public Surface {

	Sphere *head;
	Torus *body;
	Sphere *shell;


protected:
	void init();
public:
	Turtle(float* position, Game *game, float velocity, float *direction);
	~Turtle();
	void draw(GLuint programID);
	void update();
};
#endif