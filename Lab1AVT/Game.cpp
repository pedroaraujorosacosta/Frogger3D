#include "Game.h"
#include "Matrix.h"
#include "Object.h"

#include "ManagerObj.h"

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


#define CAPTION "Assignment 1"

Game::Game(int WinX, int WinY) : FOV(90), n(0.1), S(tan(FOV*0.5*(M_PI / 180)) * n), r(aspectRatio * S), l(-r), t(S), b(-t), f(20.0),
	isLeftButtonDown(false), isRightButtonDown(false), frameCount(0), totalFrames(0), startTime(0.0), windowHandle(0)
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
	createShaderProgram();
	createBufferObjects();
	//x esquerda - direita
	//y cima - baixo
	//z near - far -> nao afecta sem perspective

	managerObj = new ManagerObj(this);

	float posFrog[] = { 0.0, -7.0, -2.0 };
	float directionFrog[3] = { 0.0, 1.0, 0.0 };
	frog = new Frog(posFrog, this, 0.0, directionFrog);

	// setup camera
	S = tan(FOV*0.5*(M_PI / 180)) * n;
	r = aspectRatio * S, l = -r;
	t = S, b = -t;
	cam = new Camera(this, t, b, n, f, l, r, FOV, S);
}

// light direction
float ldir[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
Vector lightDir(ldir, 4);
float lpos[4] = { 0.0f, 0.0f, 14.0f, 1.0f };
Vector lightPos(lpos, 4);
float sdir[4] = { -4.0f, -6.0f, -2.0f, 0.0f };
Vector spotDir(sdir, 4);


void Game::draw(GLuint programID) {
	++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelViewStack.push();

	projectionStack.push();
	
	cam->setCamera();

	glUseProgram(ProgramId);
	// transform light to camera space and send it to GLSL
	Vector res(4);
	res = *modelViewStack.getTop() * lightPos;
	//res.normalize();

	glUniform4fv(lPos_uniformId, 1, res.v);
	//shader->setUniform("l_pos", res.v);

	// So usado para luzes direccionais
	/*shader->setBlockUniform("Lights", "l_dir", res.v);

	res = *modelViewStack.getTop() * lightPos;
	shader->setBlockUniform("Lights", "l_pos", res.v);

	res = *modelViewStack.getTop() * spotDir;
	shader->setBlockUniform("Lights", "l_spotDir", res.v);*/
	
	managerObj->draw();

	frog->draw(ProgramId);

	glUseProgram(0);

	projectionStack.pop();
	modelViewStack.pop();

	glutSwapBuffers();
}

void Game::reset() {}

void Game::update() 
{
	managerObj->update();
	frog->update();
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

void Game::createShaderProgram() {

	shader->init();
	shader->loadShader(VSShaderLib::VERTEX_SHADER, "pointlight.vert");
	shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "pointlight.frag");

	// set semantics for the shader variables
	/*shader->setProgramOutput(0, "colorOut");
	shader->setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
	shader->setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
	shader->setVertexAttribName(VSShaderLib::TEXTURE_COORD_ATTRIB, "texCoord");*/

	ProgramId = shader->getProgramIndex();
	glBindFragDataLocation(ProgramId, 0, "colorOut");
	glBindAttribLocation(ProgramId, VSShaderLib::VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(ProgramId, VSShaderLib::NORMAL_ATTRIB, "normal");
	glBindAttribLocation(ProgramId, VSShaderLib::TEXTURE_COORD_ATTRIB, "texCoord");
	
	//shader->prepareProgram();

	glLinkProgram(ProgramId);

	pvm_uniformId = glGetUniformLocation(ProgramId, "m_pvm");
	vm_uniformId = glGetUniformLocation(ProgramId, "m_viewModel");
	normal_uniformId = glGetUniformLocation(ProgramId, "m_normal");
	lPos_uniformId = glGetUniformLocation(ProgramId, "l_pos");

    printf("InfoLog for Hello World Shader\n%s\n\n", shader->getAllInfoLogs().c_str());

	
	/*if (readShaderProgram("vtxShader.vsh", &VtxShader))
	{
		VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderId, 1, &VtxShader, 0);
		glCompileShader(VertexShaderId);
		checkShaderCompilation(VertexShaderId);
	}

	if (readShaderProgram("fragShader.fsh", &FragShader))
	{
		FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShaderId, 1, &FragShader, 0);
		glCompileShader(FragmentShaderId);
		checkShaderCompilation(VertexShaderId);
	}

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, MY_VERTEX_COORD_ATTRIB, "in_Position");

	glLinkProgram(ProgramId);
	checkProgramLinkage(ProgramId, VertexShaderId, FragmentShaderId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");*/

	checkOpenGLError("ERROR: Could not create shaders.");
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

void Game::destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

void Game::cleanup()
{
	if (shader) { delete shader; shader = 0; }
	// TODO: apagar estas duas linhas seguintes
	/*if (FragShader) { delete FragShader; FragShader = 0; }
	if (VtxShader) { delete VtxShader; VtxShader = 0; }*/
	destroyShaderProgram();
	destroyBufferObjects();

	if (frog) { delete frog; frog = 0; }
	if (cam) { delete cam; cam = 0; }
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
	return pvm_uniformId;
}

Matrix Game::getVM()
{
	return *modelViewStack.getTop();
}

GLuint Game::getVMid()
{
	return vm_uniformId;
}

GLuint Game::getIVMid()
{
	return normal_uniformId;
}

VSShaderLib* Game::getShader()
{
	return shader;
}

GLuint Game::getProgramID()
{
	return ProgramId;
}

void Game::keyboardUp(unsigned char key, int x, int y) 
{
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

void Game::keyboard(unsigned char key, int x, int y)
{
	float front[3] = { 0.0, 1.0, 0.0 };
	float back[3] = { 0.0, -1.0, 0.0 }; 
	float left[3] = { -1.0, 0.0, 0.0 };
	float right[3] = { 1.0, 0.0, 0.0 };
	
	switch (key) {
		case 'q':
		case 'Q':
			frog->move(front);
			break;
		case 'a':
		case 'A':
			frog->move(back);
			break;
		case 'o':
		case 'O':
			frog->move(left);
			break;
		case 'p':
		case 'P':
			frog->move(right);
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
 