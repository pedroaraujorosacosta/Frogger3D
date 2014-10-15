#ifndef __GAME_H__
#define __GAME_H__

#include "IComponent.h"
#include "Stack.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define MY_VERTEX_COORD_ATTRIB 0
#define MY_NORMAL_ATTRIB 1
#define MY_TEXTURE_COORD_ATTRIB 2
//#define M_PI atan(1)*4

class ManagerObj;
class Camera;
class Frog;
class VSShaderLib;

class Game : IComponent {

	int winX;
	int winY;
	double aspectRatio;
	Stack modelViewStack;
	Stack projectionStack;
	double startTime; 
	int windowHandle;
	unsigned int frameCount;
	unsigned int totalFrames;
	Frog *frog;
	Camera *cam;
	VSShaderLib *shader;
	ManagerObj *managerObj;

	double FOV;
	double n;
	double S;
	double r, l;
	double t, b;
	double f;

	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	//GLuint UniformId;
	GLint pvm_uniformId;
	GLint vm_uniformId;
	GLint normal_uniformId;
	GLint lPos_uniformId;
	char *VtxShader;
	char *FragShader;
	
	bool isLeftButtonDown;
	bool isRightButtonDown;
	
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
	void mouseFunc(int button, int state, int x, int y);
	void passiveMouseFunc(int x, int y);
	void mouseMotionFun(int x, int y);

	Matrix getPVM();
	GLuint getPVMid();
	Matrix getVM();
	GLuint getVMid();
	GLuint getProgramID();
	Stack* getModelViewStack();
	Stack* getProjectionStack();
	Frog* getFrog();
	VSShaderLib* getShader();
};

#endif