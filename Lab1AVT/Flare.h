#ifndef __FLARE_H
#define __FLARE_H

#include <vector>
#include <GL/glew.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "IComponent.h"

#define FLARE_MAXELEMENTSPERFLARE 15
#define FLARE_MINELEMENTSPERFLARE 8
#define FLARE_RANGE(A,B)    ( (rand()%((B)-(A)+1)) + (A) )
#define FLARE_FRANGE(A,B)   ( ((float)(rand()&0xffffff)/(float)0xfffffe)*((B)-(A)) + (A) )
#define FLARE_MINCOLOUR        MAKEID(140, 100, 50, 100)
#define FLARE_MAXCOLOUR        MAKEID(255, 255, 200, 255)
#define FLARE_MINELEMENTSPERFLARE         8
#define FLARE_MAXSIZE                   0.3f
#define MAKEID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | ((d)<<0))
#define HEIGHTFROMWIDTH(w)  ((320*(w)*glutGet(GLUT_WINDOW_HEIGHT))/(240*glutGet(GLUT_WINDOW_WIDTH)))
#define isqrt(x)        (int)((double)(x))


typedef struct TEXTURE_DEF
{
	char    *filename;
	int     width;
	int     height;
	void    *pixels;
	unsigned char   *memory;        // file buffer; free this when done with texture
}
TEXTURE_DEF;

class Game;
class Quad;

struct NamedTexture
{
	char filename[200];
	GLuint texID;
	int tUnit;
	NamedTexture(char *filename, GLuint texID, int tUnit) 
	{
		strcpy_s(this->filename, 200, filename); this->texID = texID; this->tUnit = tUnit;
	}
};

class Flare : public IComponent
{
	Game *game;
	std::vector<NamedTexture*> texs;
	bool hasBeenLoaded(char *filename);
	void getNamedTexture(char *filename, GLuint *texID, int *tUnit);
	GLuint TM_setTexture(TEXTURE_DEF *tex);
	int xFlare;
	int yFlare;

public:
	Flare(Game *game);
	~Flare();
	void draw();
	void update(float dt);
	void reset();

	void getXYFlare(int *xFlare, int *yFlare);
	void setXYFlare(int xFlare, int yFlare);
};

#endif
