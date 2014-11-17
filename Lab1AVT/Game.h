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

class ManagerObj;
class Camera;
class Frog;
class VSShaderLib;
class ManagerLight;
class Light;
class VSFontLib;
class Flare;
class ParticleSystem;

class Game : IComponent {

	enum GameState {WIN, LOSE, PLAYING};
	// specifies what type of alpha test we want. AT_NONE for no alpha testing, AT_OPAQUE to pass opaque fragments only, etc.
	enum AlphaTest {AT_NONE, AT_OPAQUE, AT_TRANSLUCID}; 

	int winX;
	int winY;
	double aspectRatio;
	Stack modelViewStack;
	Stack projectionStack;
	double time;
	double startTime;
	int windowHandle;
	unsigned int frameCount;
	unsigned int totalFrames;
	Frog *frog;
	Camera *cam;
	VSShaderLib *shader;
	ManagerObj *managerObj;
	ManagerLight *managerLight;
	VSFontLib *vsfl;

	GameState gameState;
	int gamePoints;

	double FOV;
	double n;
	double S;
	double r, l;
	double t, b;
	double f;

	int pIndex;
	GLuint programId[3];
	GLuint VertexShaderId[3], FragmentShaderId[3];
	//GLuint UniformId;
	GLint pvm_uniformId[3];
	GLint vm_uniformId[3];
	GLint normal_uniformId[3];
	GLint lPos_uniformId[3];
	GLint lDir_uniformId[3];
	char *VtxShader;
	char *FragShader;
	bool isLeftButtonDown;
	bool isRightButtonDown;
	bool isFogOn;
	bool isFlareOn;
	char keyDown;
	GLuint TextureArray[5];
	unsigned int uiWinID;
	unsigned int uiLoseID;

	Flare* flare;
	ParticleSystem *ps;

	void setupGLUT(int argc, char* argv[]);
	void setupGLEW();
	void setupOpenGL();
	void createShaderPrograms();
	bool readShaderProgram(const char *filename, char **output);
	void destroyShaderPrograms();
	void destroyShaderProgram(int pIndex);
	void checkProgramLinkage(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId);
	void checkShaderCompilation(GLuint shaderId);
	unsigned int getStreamSize(std::ifstream &ifs);
	void destroyTextureObjects();
	void resetProgram();
	void createShaderProgram(int pIndex);
	void loadShader(int pIndex, unsigned int ShaderType, char *filename);
	void renderHUD();
	void setAlphaTest(AlphaTest alphaTest);
	void setFog();
	void clearFog();
	void drawFlare();
	void drawParticleSystem();

public:
	Game(int WinX, int WinY);
	~Game();
	void init(int argc, char* argv[]);

	void draw();
	void reset();
	void update(float dt);

	void reshape(int w, int h);
	void cleanup();
	void timer(int value);
	void keyboard(unsigned char key, int x, int y);
	void keyboardUp(unsigned char key, int x, int y);
	void mouseFunc(int button, int state, int x, int y);
	void passiveMouseFunc(int x, int y);
	void mouseMotionFun(int x, int y);

	void setProgramIndex(int pIndex);

	Matrix getPVM();
	GLuint getPVMid();
	Matrix getVM();
	GLuint getVMid();
	GLuint getIVMid();
	Stack* getModelViewStack();
	Stack* getProjectionStack();
	Frog* getFrog();
	GLuint getShader();
	GLuint getLPosID();
	Light* getSpotLight();
	ManagerObj* getManagerObj();
	Camera* getCamera();
	bool isGameWon();
	bool isGameLost();
	bool isGamePlaying();
	int getStartTime();

	void winGame();
	void LoadBMPTexture(unsigned int *textureArray, const char * bitmap_file, int ID);
};

#endif