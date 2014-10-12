#ifndef __GAME_H__
#define __GAME_H__

#include "IComponent.h"
#include "Stack.h"
#include "Frog.h"


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define MY_VERTEX_COORD_ATTRIB 0
#define MY_NORMAL_ATTRIB 1
#define MY_TEXTURE_COORD_ATTRIB 2
//#define M_PI atan(1)*4

class Object;

class Game : IComponent {

	int winX;
	int winY;
	double aspectRatio = (double)winX / winY;
	Stack modelViewStack;
	Stack projectionStack;
	double startTime = 0.0; 
	int windowHandle = 0;
	unsigned int frameCount = 0;
	unsigned int totalFrames = 0;
	std::vector<Object*> objects;
	Frog *frog;

	double FOV;
	double n;
	double S;
	double r, l;
	double t, b;
	double f;

	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLuint UniformId;
	char *VtxShader;
	char *FragShader;
	
	
	void setupGLUT(int argc, char* argv[]);
	void setupGLEW();
	void setupOpenGL();
	void createShaderProgram();
	bool readShaderProgram(const char *filename, char **output);
	void destroyShaderProgram();
	void checkProgramLinkage(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId);
	void checkShaderCompilation(GLuint shaderId);
	unsigned int getStreamSize(std::ifstream &ifs);
	void createBufferObjects();
	void destroyBufferObjects();

public:
	Game(int WinX, int WinY);
	~Game();
	void init(int argc, char* argv[]);

	void draw(GLuint programID);
	void reset();
	void update();

	void reshape(int w, int h);
	void cleanup();
	void timer(int value);
	void keyboard(unsigned char key, int x, int y);
	void keyboardUp(unsigned char key, int x, int y);

	Matrix getPVM();
	GLuint getPVMid();
	Stack* getModelViewStack();
	Stack* getProjectionStack();
	Frog* getFrog();
};

#endif