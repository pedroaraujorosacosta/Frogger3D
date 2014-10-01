///////////////////////////////////////////////////////////////////////
//
// 
// (c) 2014 by João Madeiras Pereira
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "cube.h"

#define CAPTION "Exercise 1"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
unsigned int TotalFrames = 0;
double startTime = 0.0;

#define VERTEX_COORD_ATTRIB 0
#define NORMAL_ATTRIB 1
#define TEXTURE_COORD_ATTRIB 2
#define M_PI atan(1)*4

const double TARGET_FPS = 60.0;
const double TIME_PER_FRAME = 1000 / TARGET_FPS;
GLuint VaoId, VboId[4];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;
char *VtxShader;
char *FragShader;


/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec4 in_Position;\n"
	"uniform mat4 Matrix;\n"
	"out vec4 color;\n"

	"void main(void)\n"
	"{\n"
	"	color = in_Position;\n"
	"	gl_Position = Matrix * in_Position;\n"

	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = color;\n"
	"}\n"
};

unsigned int getStreamSize(std::ifstream &ifs)
{
	std::streampos beg, end;
	beg = ifs.tellg();
	ifs.seekg(0, std::ios::end);
	end = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	return (unsigned int)(end-beg);
}

bool readShaderProgram(const char *filename, char **output)
{
	std::ifstream ifs(filename, std::ifstream::in|std::ifstream::binary);
	if (ifs.is_open())
	{
		unsigned int size = getStreamSize(ifs);
		*output = new char[size+1];
		ifs.read(*output, size);
		(*output)[size] = 0;
		ifs.close();
		return true;
	}
	else 
		return false;
}

void cleanup();

void checkShaderCompilation(GLuint shaderId)
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

void checkProgramLinkage(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId)
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

void createShaderProgram()
{
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

	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");

	glLinkProgram(ProgramId);
	checkProgramLinkage(ProgramId, VertexShaderId, FragmentShaderId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs


void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(4, VboId);

	//vertex coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	// in this case, the buffer is allocated and nothing is transferred to it until bufferSubData is called
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	//normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_ATTRIB);
	glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);

	//texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
	glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex, GL_STATIC_DRAW);

	// unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(NORMAL_ATTRIB);
	glDisableVertexAttribArray(TEXTURE_COORD_ATTRIB);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(NORMAL_ATTRIB);
	glDisableVertexAttribArray(TEXTURE_COORD_ATTRIB);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(4, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

typedef GLfloat Matrix[16];

const Matrix I = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
}; // Row Major (GLSL is Column Major)

const Matrix M = {
	1.0f, 0.0f, 0.0f, -1.0f,
	0.0f, 1.0f, 0.0f, -1.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
}; // Row Major (GLSL is Column Major)

// these globals are used in the view and perspective projection calculations. They're global
// so we may change them with the keyboard handler
double FOV = 90;
double aspectRatio = (double)WinX / WinY;
double n = 2.0;
double S = tan(FOV*0.5*(M_PI / 180)) * n;
double r = aspectRatio * S, l = -r;
double t = S, b = -t;
double f = 4.0;
GLfloat eye[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat right[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
GLfloat up[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
GLfloat lookPoint[4] = { 0.0f, 0.0f, -1.0f, 1.0f };
double boxTranslationZ = -2.0;

void multMatrix4(const Matrix a, const Matrix b, Matrix &c)
{
	// null the result matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			c[j + 4 * i] = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				c[j + 4 * i] += a[k + 4 * i] * b[j + 4 * k];
			}
		}
	}
}

GLfloat norm(GLfloat *vector, int size)
{
	GLfloat norm = 0;
	for (int i = 0; i < size; i++)
		norm += vector[i] * vector[i];

	return sqrt(norm);
}

void normalize(GLfloat *vector, int size)
{
	GLfloat magnitude = norm(vector, size);
	for (int i = 0; i < size; i++)
		vector[i] = vector[i] / magnitude;
}

GLfloat dotProduct(GLfloat *u, GLfloat *v, int size)
{
	GLfloat product = 0.0f;
	for (int i = 0; i < size; i++)
		product += u[i] * v[i];

	return product;
}

void lookAt(GLfloat *right, GLfloat *up, GLfloat *eye, GLfloat *lookPoint, Matrix &view)
{
	GLfloat lookAt[4];

	// build the lookAt vector
	for (int i = 0; i < 4; i++)
		lookAt[i] = eye[i] - lookPoint[i];
	normalize(lookAt, 4);
	
	// null the matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			view[j + 4 * i] = 0.0f;

	// build the view matrix
	for (int i = 0; i < 3; i++)
	{
		view[i] = right[i];
		view[i + 4] = up[i];
		view[i + 8] = lookAt[i];
	}
	view[3] = - dotProduct(right, eye, 3);
	view[3 + 4] = - dotProduct(up, eye, 3);
	view[3 + 8] = - dotProduct(lookAt, eye, 3);
	view[15] = 1.0f;
}

void perspective(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
	GLfloat nearPlane, GLfloat farPlane, Matrix &projection)
{
	// null the matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			projection[j + 4 * i] = 0.0f;

	projection[0] = 2 * nearPlane / (right - left);
	projection[2] = (right + left) / (right - left);
	projection[5] = 2 * nearPlane / (top - bottom);
	projection[6] = (top + bottom) / (top - bottom);
	projection[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
	projection[11] = -2 * farPlane * nearPlane / (farPlane - nearPlane);
	projection[14] = -1.0f;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
		f+=0.1;
	}
	else if (key == 'a')
	{
		f-=0.1;
	}
	else if (key == 'w')
	{
		n += 0.1;
	}
	else if (key == 's')
	{
		n -= 0.1;
	}
	else if (key == 'e')
	{
		boxTranslationZ += 0.1;
	}
	else if (key == 'd')
	{
		boxTranslationZ -= 0.1;
	}
}

void renderScene()
{
	S = tan(FOV*0.5*(M_PI / 180)) * n;
	r = aspectRatio * S, l = -r;
	t = S, b = -t;

	Matrix M = {
		1.0f, 0.0f, 0.0f, -0.5f,
		0.0f, 1.0f, 0.0f, -0.5f,
		0.0f, 0.0f, 1.0f, boxTranslationZ,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// View matrix
	Matrix V;
	// ModelView matrix
	Matrix VM;

	// build the View matrix and place it in V
	lookAt(right, up, eye, lookPoint, V);
	
	// build the ModelView matrix, place it in VM
	multMatrix4(V, M, VM);

	std::cout << "n: " << n << " f: " << f << " FOV: " << FOV << std::endl;
	std::cout << "l: " << l << " r: " << r << " t: " << t << " b: " << b << std::endl;
	std::cout << "aspect: " << aspectRatio << " temp: " << boxTranslationZ << std::endl;

	Matrix P;
	Matrix PVM;

	// build the perspective Projection matrix
	perspective(l, r, b, t, n, f, P);

	// build the ModelViewProjection matrix
	multMatrix4(P, VM, PVM);

	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	/*glUniformMatrix4fv(UniformId, 1, GL_TRUE, I);
	glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid*)0);*/

	glUniformMatrix4fv(UniformId, 1, GL_TRUE, PVM);
	glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	if (FragShader) delete FragShader;
	if (VtxShader) delete VtxShader;
	destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene();
	glutSwapBuffers();
}

void idle()
{
	double endFrameTime, endRenderingTime, wasteTime;

	glutPostRedisplay();

	endFrameTime = startTime + (TotalFrames + FrameCount + 1) * TIME_PER_FRAME;
	endRenderingTime = glutGet(GLUT_ELAPSED_TIME);
	wasteTime = endFrameTime - endRenderingTime;

	//std::cout << wasteTime << " " << endFrameTime << " " << endRenderingTime << " " << TIME_PER_FRAME << std::endl;

	if (wasteTime > 0)
		Sleep((DWORD)wasteTime);
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	aspectRatio = (double)WinX / WinY;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());

	TotalFrames += FrameCount;
	FrameCount = 0; 
	glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboardFunc);
}

void setupOpenGL() {
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

void setupGLEW() {
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

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	startTime = glutGet(GLUT_ELAPSED_TIME);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////