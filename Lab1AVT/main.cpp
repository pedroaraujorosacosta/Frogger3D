#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "Stack.h"
#include "Matrix.h"

#include "Game.h"

int WinX = 1000, WinY = 800;
Game game(WinX, WinY);


#define VERTEX_COORD_ATTRIB 0
#define NORMAL_ATTRIB 1
#define TEXTURE_COORD_ATTRIB 2

const double TARGET_FPS = 60.0;
const double TIME_PER_FRAME = 1000 / TARGET_FPS;
//GLuint VaoId, VboId[4];
float angle = 0;
GLuint rotation = 0;
long int CurrentTime;

/////////////////////////////////////////////////////////////////////// SCENE

// these globals are used in the view and perspective projection calculations. They're global
// so we may change them with the keyboard handler
GLfloat eye[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; 
GLfloat right[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; //u 
GLfloat up[4] = { 0.0f, 1.0f, 0.0f, 0.0f }; //n
GLfloat lookPoint[4] = { 0.0f, 0.0f, -1.0f, 1.0f }; 
double boxTranslationZ = -2.0;


void keyboardFunc(unsigned char key, int x, int y)
{
	game.keyboard(key, x, y);
}

void keyboardUpFunc(unsigned char key, int x, int y)
{
	game.keyboardUp(key, x, y);
}

void passiveMouseFunc(int x, int y)
{
	game.passiveMouseFunc(x, y);
}

void mouseMotionFunc(int x, int y)
{
	game.mouseMotionFun(x, y);
}

void mouseFunc(int button, int state, int x, int y)
{
	game.mouseFunc(button, state, x, y);
}
/////////////////////////////////////////////////////////////////////// CALLBACKS

void display()
{
	static long int oldTime = glutGet(GLUT_ELAPSED_TIME);
	CurrentTime = glutGet(GLUT_ELAPSED_TIME);

	long int dt = CurrentTime - oldTime;

	game.update(dt/1000.0f);
	game.draw();

	oldTime = CurrentTime;
}

void idle()
{
	//glutPostRedisplay();
}

void reshape(int w, int h)
{
	game.reshape(w, h);
}

void timer(int value)
{
	game.timer(value);
	glutTimerFunc(1000, timer, 0);
	
}

void frameTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(TIME_PER_FRAME, frameTimer, 0);
}

void cleanup()
{
	game.cleanup();
}
/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(TIME_PER_FRAME, frameTimer, 0);
	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardUpFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutPassiveMotionFunc(passiveMouseFunc);
}

void init(int argc, char* argv[])
{
	game.init(argc, argv);
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

