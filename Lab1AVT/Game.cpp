#include "Game.h"
#include "Matrix.h"
#include "Object.h"

#include "ManagerObj.h"
#include "ManagerLight.h"

#include "River.h"
#include "Road.h"
#include "Turtle.h"
#include "FloatingLog.h"
#include "Bus.h"
#include "Car.h"

#include "Frog.h"
#include "Camera.h"
#include "vsShaderLib.h"
#include "Vector.h"
#include "Light.h"
#include "TGA.h"
#include "FontLib.h"
#include <cctype>

#define LIFES 5

#define CAPTION "Assignment 1"

Game::Game(int WinX, int WinY) : FOV(90), n(0.1), S(tan(FOV*0.5*(M_PI / 180)) * n), r(aspectRatio * S), l(-r), t(S), b(-t), f(30.0),
	isLeftButtonDown(false), isRightButtonDown(false), frameCount(0), totalFrames(0), startTime(0.0), windowHandle(0),
	pIndex(0), keyDown('\0')
{
	winX = WinX;
	winY = WinY;
}

Game::~Game() 
{
	//delete frog;
}

void Game::init(int argc, char* argv[])
{
	shader = new VSShaderLib();
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderPrograms();
	createBufferObjects();
	//x esquerda - direita
	//y cima - baixo
	//z near - far -> nao afecta sem perspective

	managerObj = new ManagerObj(this);
	managerLight = new ManagerLight(this);
	managerLight->toggleSpotLight();

	float posFrog[] = { 3.0, -13.0, -2.0 };
	float directionFrog[3] = { 0.0, 1.0, 0.0 };
	frog = new Frog(posFrog, this, 0.0, directionFrog, LIFES);

	/*
	  Remover isto, foi so para testar o operador< dos objectos, para depois fazer o sort.
	Frog fr(posFrog, this, 0.0, directionFrog, LIFES);
	float posR[] = { 3.0, -13.1, -2.0 };
	River riv(posR, this);
	std::cout << (fr < riv) << std::endl;*/

	// setup camera
	S = tan(FOV*0.5*(M_PI / 180)) * n;
	r = aspectRatio * S, l = -r;
	t = S, b = -t;
	cam = new Camera(this, t, b, n, f, l, r, FOV, S);

	//setup textures
	glGenTextures(4, TextureArray);
	TGA_Texture(TextureArray, "water.tga", 0, true);
	TGA_Texture(TextureArray, "ground.tga", 1, true);
	TGA_Texture(TextureArray, "grass.tga", 2, true);
	TGA_Texture(TextureArray, "tree.tga", 3, false);

	vsfl = new VSFontLib(this);
	vsfl->load("arial");

	uiWinID = vsfl->genSentence();

	uiLoseID = vsfl->genSentence();

	//game state
	this->gameState = PLAYING; 
}

// light direction
float ldir[4] = { 1.0f, 1.0f, -1.0f, 0.0f };
Vector lightDir(ldir, 4);
float lpos[4] = { 0.0f, 0.0f, 14.0f, 1.0f };
Vector lightPos(lpos, 4);
float sdir[4] = { -4.0f, -6.0f, -2.0f, 0.0f };
Vector spotDir(sdir, 4);

void Game::setProgramIndex(int pIndex)
{
	glUseProgram(programId[pIndex]);
	this->pIndex = pIndex;
}

void Game::resetProgram()
{
	glUseProgram(0);
	pIndex = 0;
}


void Game::draw() {
	++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelViewStack.push();

	projectionStack.push();

	cam->setCamera();

	setProgramIndex(0);

	managerLight->illuminate();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[2]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureArray[3]);

	// activate alpha test to draw opaques only
	setAlphaTest(AT_OPAQUE);

	managerObj->draw();

	if (frog->getLife() > 0)
		frog->draw();
	else
		this->gameState = LOSE;

	// activate alpha test to draw translucids only
	setAlphaTest(AT_TRANSLUCID);

	managerObj->draw();

	if (frog->getLife() > 0)
		frog->draw();
	else
		this->gameState = LOSE;

	// deactivate alpha test
	setAlphaTest(AT_NONE);

	resetProgram();

	projectionStack.pop();
	modelViewStack.pop();

	// Render the UI
	renderHUD();

	glutSwapBuffers();
}

void Game::renderHUD()
{
	char str[256];

	setProgramIndex(1);

	unsigned int livesSentenceID;
	livesSentenceID = vsfl->genSentence();
	sprintf_s(str, "Lives: %d", frog->getLife());
	vsfl->prepareSentence(livesSentenceID, str);

	vsfl->setColor(1.0f, 0.0f, 0.0f, 1.0f);
	vsfl->renderSentence(30, 10, livesSentenceID);

	unsigned int pointsSentenceID;
	pointsSentenceID = vsfl->genSentence();
	sprintf_s(str, "Points: %d", gamePoints);
	vsfl->prepareSentence(pointsSentenceID, str);
	vsfl->renderSentence(winX/2+winX/4, 10, pointsSentenceID);

	if (gameState == WIN)
	{
		vsfl->prepareSentence(uiWinID, "You won! Congrats!");
		vsfl->renderSentence(0.35f*winX, 0.45f*winY, uiWinID);
	}
	else if (gameState == LOSE)
	{
		vsfl->prepareSentence(uiLoseID, "You lost! Try again!");
		vsfl->renderSentence(0.35f*winX, 0.45f*winY, uiLoseID);
	}
}

void Game::winGame() {
	this->gameState = WIN;
	this->gamePoints = 10000000 / (glutGet(GLUT_ELAPSED_TIME) - startTime);
	std::cout << this->gamePoints << std::endl;
}

void Game::reset() 
{
	float posFrog[] = { 3.0, -13.0, -2.0 };
	frog->setPosition(posFrog);
	float directionFrog[] = { 0.0, 1.0, 0.0 };
	frog->setDirection(directionFrog);
	frog->setVelocity(0.0);
	frog->setLife(LIFES);
	this->gamePoints = 0;
	this->gameState = PLAYING;

	//velocidades dos objetos
	managerObj->reset();

	startTime = glutGet(GLUT_ELAPSED_TIME);
}

void Game::update(float dt) 
{
	if (gameState == PLAYING)
	{
		managerObj->update(dt, frog);
		//corre primeiro e diz se houve um crash
		frog->update(dt);
		//se houver um crash o frog vai fazer update para o inicio
	}
}

void Game::reshape(int w, int h)
{
	winX = w;
	winY = h;
	aspectRatio = (double)winX / winY;
	cam->reshape(w, h);
}

void Game::setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	startTime = glutGet(GLUT_ELAPSED_TIME);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(winX, winY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	windowHandle = glutCreateWindow(CAPTION);
	if (windowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Game::setupGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void Game::setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}


void Game::createShaderPrograms() 
{
	createShaderProgram(0);
	createShaderProgram(1);

	checkOpenGLError("ERROR: Could not create shaders.");
}

void Game::loadShader(int pIndex, unsigned int ShaderType, char *filename)
{
	if (ShaderType == VSShaderLib::VERTEX_SHADER)
	{
		if (readShaderProgram(filename, &VtxShader))
		{
			VertexShaderId[pIndex] = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(VertexShaderId[pIndex], 1, &VtxShader, 0);
			glAttachShader(programId[pIndex], VertexShaderId[pIndex]);
			glCompileShader(VertexShaderId[pIndex]);
			checkShaderCompilation(VertexShaderId[pIndex]);
		}
	}
	else
	{
		if (readShaderProgram(filename, &FragShader))
		{
			FragmentShaderId[pIndex] = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(FragmentShaderId[pIndex], 1, &FragShader, 0);
			glAttachShader(programId[pIndex], FragmentShaderId[pIndex]);
			glCompileShader(FragmentShaderId[pIndex]);
			checkShaderCompilation(FragmentShaderId[pIndex]);
		}
	}
	if (FragShader) { delete FragShader; FragShader = 0; }
	if (VtxShader) { delete VtxShader; VtxShader = 0; }
}

void Game::createShaderProgram(int pIndex)
{
	programId[pIndex] = glCreateProgram();

	switch (pIndex) {
	case 0:
		loadShader(0, VSShaderLib::VERTEX_SHADER, "pointlight.vert");
		loadShader(0, VSShaderLib::FRAGMENT_SHADER, "pointlight.frag");
		break;
	case 1:
		loadShader(1, VSShaderLib::VERTEX_SHADER, "fontshade.vert");
		loadShader(1, VSShaderLib::FRAGMENT_SHADER, "fontshade.frag");
		break;
		/*	case 2:
		loadShader(2, VSShaderLib::VERTEX_SHADER, "spotlight.vert");
		loadShader(2, VSShaderLib::FRAGMENT_SHADER, "spotlight.frag");
		break;*/
	}

	glBindFragDataLocation(programId[pIndex], 0, "colorOut");
	glBindAttribLocation(programId[pIndex], VSShaderLib::VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(programId[pIndex], VSShaderLib::NORMAL_ATTRIB, "normal");
	glBindAttribLocation(programId[pIndex], VSShaderLib::TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(programId[pIndex]);
	checkProgramLinkage(programId[pIndex], VertexShaderId[pIndex], FragmentShaderId[pIndex]);

	pvm_uniformId[pIndex] = glGetUniformLocation(programId[pIndex], "m_pvm");
	vm_uniformId[pIndex] = glGetUniformLocation(programId[pIndex], "m_viewModel");
	normal_uniformId[pIndex] = glGetUniformLocation(programId[pIndex], "m_normal");
}


bool Game::readShaderProgram(const char *filename, char **output) {
	std::ifstream ifs(filename, std::ifstream::in | std::ifstream::binary);
	if (ifs.is_open())
	{
		unsigned int size = getStreamSize(ifs);
		*output = new char[size + 1];
		ifs.read(*output, size);
		(*output)[size] = 0;
		ifs.close();
		return true;
	}
	else
		return false;
}

//Size of input file
unsigned int Game::getStreamSize(std::ifstream &ifs)
{
	std::streampos beg, end;
	beg = ifs.tellg();
	ifs.seekg(0, std::ios::end);
	end = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	return (unsigned int)(end - beg);
}

void Game::checkShaderCompilation(GLuint shaderId)
{
	GLint isCompiled = 0;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

		//We don't need the shader anymore.
		glDeleteShader(shaderId);

		//Use the infoLog as you see fit.
		std::cout << &infoLog[0] << std::endl;

		cleanup();
		exit(EXIT_FAILURE);
	}
}

void Game::checkProgramLinkage(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId)
{
	GLint isLinked = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(programId);
		//Don't leak shaders either.vtxShader
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		//Use the infoLog as you see fit.
		std::cout << &infoLog[0] << std::endl;

		cleanup();
		exit(EXIT_FAILURE);
	}
}

void Game::destroyShaderPrograms()
{
	destroyShaderProgram(0);
	destroyShaderProgram(1);
}

void Game::destroyShaderProgram(int pIndex)
{
	glUseProgram(0);
	glDetachShader(programId[pIndex], VertexShaderId[pIndex]);
	glDetachShader(programId[pIndex], FragmentShaderId[pIndex]);

	glDeleteShader(FragmentShaderId[pIndex]);
	glDeleteShader(VertexShaderId[pIndex]);
	glDeleteProgram(programId[pIndex]);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

void Game::cleanup()
{
	if (shader) { delete shader; shader = 0; }
	// TODO: apagar estas duas linhas seguintes
	/*if (FragShader) { delete FragShader; FragShader = 0; }
	if (VtxShader) { delete VtxShader; VtxShader = 0; }*/
	destroyShaderPrograms();
	destroyBufferObjects();

	if (frog) { delete frog; frog = 0; }
	if (cam) { delete cam; cam = 0; }

	vsfl->deleteSentence(uiWinID);
	vsfl->deleteSentence(uiLoseID);
}

void Game::createBufferObjects() {
	//TODO chamar Object para todos os objects
}

void Game::destroyBufferObjects() {
	//TODO chamar ~Object para todos os objects
}

void Game::timer(int value) {
	std::ostringstream oss;
	oss << CAPTION << ": " << frameCount << " FPS @ (" << winX << "x" << winY << ")";
	std::string s = oss.str();
	glutSetWindow(windowHandle);
	glutSetWindowTitle(s.c_str());

	totalFrames += frameCount;
	frameCount = 0;
}

Matrix Game::getPVM()
{
	return *projectionStack.getTop() * *modelViewStack.getTop();
}

GLuint Game::getPVMid() 
{
	return pvm_uniformId[pIndex];
}

Matrix Game::getVM()
{
	return *modelViewStack.getTop();
}

GLuint Game::getVMid()
{
	return vm_uniformId[pIndex];
}

GLuint Game::getIVMid()
{
	return normal_uniformId[pIndex];
}

GLuint Game::getShader()
{
	return programId[pIndex];
}

GLuint Game::getLPosID()
{
	return lPos_uniformId[pIndex];
}

void Game::keyboardUp(unsigned char key, int x, int y) 
{
	if (tolower(key) == tolower(keyDown))
	{
		keyDown = '\0';

		switch (key) {
		case 'q':
		case 'Q':
		case 'a':
		case 'A':
		case 'o':
		case 'O':
		case 'p':
		case 'P':
			frog->stop();
			break;
		}
	}
}

void Game::keyboard(unsigned char key, int x, int y)
{
	float front[3] = { 0.0, 1.0, 0.0 };
	float back[3] = { 0.0, -1.0, 0.0 }; 
	float left[3] = { -1.0, 0.0, 0.0 };
	float right[3] = { 1.0, 0.0, 0.0 };

	if (tolower(key) != tolower(keyDown))
	{
		keyDown = key;

		switch (key) {
		case 'q':
		case 'Q':
			if (isGamePlaying()) {
					frog->move(front);
			}
			break;
		case 'a':
		case 'A':
			if (isGamePlaying()) {
					frog->move(back);
			}
			break;
		case 'o':
		case 'O':
			if (isGamePlaying()) {
					frog->move(left);
			}
			break;
		case 'p':
		case 'P':
			if (isGamePlaying()) {
					frog->move(right);
			}
		break;
	case 'r':
	case 'R':
		this->reset();
		break;
	case '0':
		this->winGame();
			break;
		case '1':
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			cam->topCameraMode();
			break;
		case '2':
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			cam->topCameraPerspectiveMode();
			break;
		case '3':
			glutSetCursor(GLUT_CURSOR_NONE);
			cam->FPSCameraMode();
			break;
		case 'n':
		case 'N':
			managerLight->toggleDirectional();
			managerLight->toggleSpotLight();
			break;
		case 'c':
		case 'C':
			managerLight->togglePointLights();
			break;
		}
	}
}

void Game::mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		isLeftButtonDown = !isLeftButtonDown;

	if (button == GLUT_RIGHT_BUTTON)
		isRightButtonDown = !isRightButtonDown;

	if (button == GLUT_RIGHT_BUTTON && isRightButtonDown)
		frog->move();
	else if (button == GLUT_RIGHT_BUTTON && !isRightButtonDown)
		frog->stop();
}

void Game::mouseMotionFun(int x, int y)
{
	static int oldX = x;
	static int oldY = y;
	static bool warped = false;
	int newX = x;
	int newY = y;

	const int MIDDLE_X = winX / 2;
	const int MIDDLE_Y = winY / 2;

	int dx = newX - MIDDLE_X;
	int dy = newY - MIDDLE_Y;

	if (cam->isInFPSMode())
	{
		if (!warped)
		{
			glutWarpPointer(MIDDLE_X, MIDDLE_Y);
			warped = true;
		}
		else
			warped = false;
	}

	if (isLeftButtonDown)
		cam->updateDirection(dx, dy);

	oldX = newX;
	oldY = newY;
}

void Game::passiveMouseFunc(int x, int y)
{
	
}

Stack* Game::getModelViewStack() 
{
	return &modelViewStack;
}

Stack* Game::getProjectionStack()
{
	return &projectionStack;
}

Frog* Game::getFrog()
{
	return frog;
}
 
Light* Game::getSpotLight()
{
	return managerLight->getSpotLight();
}
 
ManagerObj* Game::getManagerObj()
{
	return managerObj;
}

bool Game::isGameWon()
{
	return gameState == WIN;
}

bool Game::isGameLost()
{
	return gameState == LOSE;
}

bool Game::isGamePlaying()
{
	return gameState == PLAYING;
}

void Game::setAlphaTest(AlphaTest alphaTest)
{
	GLuint loc = glGetUniformLocation(getShader(), "alphaTest");
	glUniform1i(loc, alphaTest);
	if (alphaTest == AT_TRANSLUCID)
		glDepthMask(GL_FALSE);
	else
		glDepthMask(GL_TRUE);
}

int Game::getStartTime()
{
	return startTime;
}

Camera* Game::getCamera()
{
	return cam;
}