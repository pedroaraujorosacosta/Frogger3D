#include "Game.h"
#include "Matrix.h"
#include "Object.h"
#include "Cube.h"
#include "River.h"
#include "Road.h"
#include "Car.h"
#include "Sphere.h"
#include "Frog.h"
#include "Camera.h"

#define CAPTION "Assignment 1"



Game::Game(int WinX, int WinY) : FOV(90), n(0.1), S(tan(FOV*0.5*(M_PI / 180)) * n), r(aspectRatio * S), l(-r), t(S), b(-t), f(20.0)
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
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createBufferObjects();
	//x esquerda - direita
	//y cima - baixo
	//z near - far -> nao afecta sem perspective
	float posCar[] = { 0.0, 0.0, 0.0 };
	float posriver[] = { 0.0, 3.0, -3.0};
	//objects.push_back(new River(posriver, this));
	//float posroad[] = { 0.0, -3.0, -3.0 };
	//objects.push_back(new Road(posroad, this));
	//objects.push_back(new Sphere(posroad, this, 3, 100));
	float direction[3] = { 0.0, 1.0, 0.0 };
	frog = new Frog(posCar, this, 0.0, direction);

	// setup camera
	S = tan(FOV*0.5*(M_PI / 180)) * n;
	r = aspectRatio * S, l = -r;
	t = S, b = -t;
	cam = new Camera(this, t, b, n, f, l, r, FOV, S);
}

void Game::draw(GLuint programID) {
	++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat eye[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat right[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; //u 
	GLfloat up[4] = { 0.0f, 1.0f, 0.0f, 0.0f }; //n
	GLfloat lookPoint[4] = { 0.0f, 0.0f, -1.0f, 1.0f };

	modelViewStack.push();

	projectionStack.push();
	
	cam->setCamera();
	//projectionStack.orthogonal(-10, 10, -7, 7, 0.1, 10);
	//projectionStack.perspective(l, r, b, t, n, f);
	//modelViewStack.lookAt(right, up, eye, lookPoint);
	/*modelViewStack.lookAt(eye[0], eye[1], eye[2],
		lookPoint[0], lookPoint[1], lookPoint[2],
		up[0], up[1], up[2]);*/

	for (int i = 0; i < objects.size(); i++)
		objects[i]->draw(this->ProgramId);
	frog->draw(this->ProgramId);

	projectionStack.pop();
	modelViewStack.pop();

	glutSwapBuffers();
}

void Game::reset() {}
void Game::update() {

	for (int i = 0; i < objects.size(); i++)
		objects[i]->update();
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
	if (readShaderProgram("vtxShader.vsh", &VtxShader))
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
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

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
	if (FragShader) delete FragShader;
	if (VtxShader) delete VtxShader;
	destroyShaderProgram();
	destroyBufferObjects();
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
	return UniformId;
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
			cam->topCameraMode();
			break;
		case '2':
			cam->topCameraPerspectiveMode();
			break;
		case '3':
			cam->FPSCameraMode();
			break;
	}
}

void Game::mouseFunc(int button, int state, int x, int y)
{

}

void Game::passiveMouseFunc(int x, int y)
{
	static int oldX = x;
	static int oldY = y;
	int newX = x;
	int newY = y;

	int dx = newX - oldX;
	int dy = newY - oldY;

	//std::cout << "x: " << x << " y: " << y << std::endl;
	//std::cout << "dx: " << dx << " dy: " << dy << std::endl;
	//std::cout << "oldX: " << oldX << " oldY: " << oldY << std::endl;
	cam->updateDirection(dx, dy);

	oldX = newX;
	oldY = newY;
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
 