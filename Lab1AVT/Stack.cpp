#include "Stack.h"
#include "Matrix.h"
#define M_PI 3.1415

Stack::Stack(){
	loadIdentity();
}

Stack::~Stack(){
	cleanStack();
}

void Stack::loadIdentity(){
	float id[16] = { 
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	Matrix *m = new Matrix(id, 4);

	s.push(m);
}

void Stack::loadMatrix(Matrix* matrix){
	s.push(matrix);
}
 
void Stack::cleanStack(){
	while (!s.empty()){
		Matrix *m = s.top();
		s.pop();
		if(m!=0) delete m;
	}
}


void Stack::push(){
	Matrix *p = new Matrix(*s.top());
	loadMatrix(p);
}

void Stack::pop(){
	s.pop();
}

Matrix* Stack::getTop() {
	return s.top();
}

void Stack::printStack(){
	std::stack<Matrix*> matrixPointers;

	while (!s.empty()) {
		s.top()->printMatrix();
		matrixPointers.push(s.top());
		s.pop();
	}

	while (!matrixPointers.empty()) {
		s.push(matrixPointers.top());
		matrixPointers.pop();
	}
}

void Stack::translateMatrix(float tx, float ty, float tz) {

	float t[16] = {
		1.0, 0.0, 0.0, tx,
		0.0, 1.0, 0.0, ty,
		0.0, 0.0, 1.0, tz,
		0.0, 0.0, 0.0, 1.0 };
	
	Matrix* tMatrix = new Matrix(t, 4);

	
	Matrix* nova = new Matrix(4);
	(*nova) = *s.top() * *tMatrix;
	s.pop();
	loadMatrix(nova);
}


void Stack::scaleMatrix(float sx, float sy, float sz) {

	float t[16] = {
		sx, 0.0, 0.0, 0.0,
		0.0, sy, 0.0, 0.0,
		0.0, 0.0, sz, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	Matrix* sMatrix = new Matrix(t, 4);


	Matrix* nova = new Matrix(4);
	(*nova)= *s.top() * *sMatrix;
	s.pop();
	loadMatrix(nova);
}

void Stack::rotateMatrix(float x, float y, float z, float alpha) {

	float rad = alpha * M_PI / 180;
	float c = cos(rad);
	float sen = sin(rad);
	float t = 1 - cos(rad);

	float norm = sqrt(x*x + y*y + z*z);
	float X = x / norm;
	float Y = y / norm;
	float Z = z / norm;

	float r[16] = {
		t*X*X + c, t*X*Y + sen*Z, t*X*Z - sen*Y, 0.0,
		t*X*Y - sen*Z, t*Y*Y+c, t*Y*Z+sen*X, 0.0,
		t*X*Y + sen*Y, t*Y*Z - sen*X, t*Z*Z+c, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	Matrix* rMatrix = new Matrix(r, 4);


	Matrix* nova = new Matrix(4);
	(*nova) = *s.top() * *rMatrix;
	s.pop();
	loadMatrix(nova);
}

void Stack::perspective(float left, float right, float bottom, float top, float nearPlane, float farPlane) 
{
	float projection[16];

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

	Matrix* sMatrix = new Matrix(projection, 4);


	Matrix* nova = new Matrix(4);
	(*nova) = *s.top() * *sMatrix;
	s.pop();
	loadMatrix(nova);


}

float Stack::dotProduct(float *u, float *v, int size)
{
	float product = 0.0f;
	for (int i = 0; i < size; i++)
		product += u[i] * v[i];

	return product;
}

void Stack::crossProduct(float *a, float*b, float* r) 
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

float Stack::norm(float *vector, int size)
{
	float norm = 0;
	for (int i = 0; i < size; i++)
		norm += vector[i] * vector[i];

	return sqrt(norm);
}


void Stack::normalize(float *vector, int size)
{
	float magnitude = norm(vector, size);
	for (int i = 0; i < size; i++)
		vector[i] = vector[i] / magnitude;
}

void Stack::lookAt(float eyex, float eyey, float eyez,
					float atx, float aty, float atz,
					float upx, float upy, float upz)
{
	float lookAt[3];
	float minuslookAt[3];

	// build the lookAt vector
	lookAt[0] = eyex - atx;
	lookAt[1] = eyey - aty;
	lookAt[2] = eyez - atz;
	normalize(lookAt, 3);

	for (int i = 0; i < 3; i++)
		minuslookAt[i] = -lookAt[i];

	float up[3] = { upx, upy, upz };
	float eye[3] = {eyex, eyey, eyez};

	float right[3];

	crossProduct(minuslookAt, up, right);
	normalize(right, 3);

	float newup[3];

	crossProduct(right, minuslookAt, newup);
	normalize(newup, 3);

	float view[16];

	// null the matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			view[j + 4 * i] = 0.0f;

	// build the view matrix
	for (int i = 0; i < 3; i++)
	{
		view[i] = right[i];
		view[i + 4] = newup[i];
		view[i + 8] = lookAt[i];
	}
	view[3] = -dotProduct(right, eye, 3);
	view[3 + 4] = -dotProduct(newup, eye, 3);
	view[3 + 8] = -dotProduct(lookAt, eye, 3);
	view[15] = 1.0f;

	Matrix* vMatrix = new Matrix(view, 4);

	Matrix* nova = new Matrix(4);
	(*nova) = *s.top() * *vMatrix;
	s.pop();
	loadMatrix(nova);
}

void Stack::lookAt(float *right, float *up, float *eye, float *lookPoint)
{
	float lookAt[4];

	// build the lookAt vector
	for (int i = 0; i < 4; i++)
		lookAt[i] = eye[i] - lookPoint[i];
	normalize(lookAt, 4);

	float view[16];

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
	view[3] = -dotProduct(right, eye, 3);
	view[3 + 4] = -dotProduct(up, eye, 3);
	view[3 + 8] = -dotProduct(lookAt, eye, 3);
	view[15] = 1.0f;

	Matrix* vMatrix = new Matrix(view, 4);

	Matrix* nova = new Matrix(4);
	(*nova)= *s.top() * *vMatrix;
	s.pop();
	loadMatrix(nova);
}


void Stack::orthogonal(float left, float right, float bottom, float top, float nearPlane, float farPlane) 
{
	float projection[16];

	// null the matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			projection[j + 4 * i] = 0.0f;

	projection[0] = 2 / (right - left);
	projection[3] = (right + left) / (right - left);
	projection[5] = 2 / (top - bottom);
	projection[7] = (top + bottom) / (top - bottom);
	projection[10] = -2 / (nearPlane - farPlane);
	projection[11] = (farPlane + nearPlane) / (farPlane - nearPlane);
	projection[15] = 1.0f;

	Matrix* sMatrix = new Matrix(projection, 4);


	Matrix* nova = new Matrix(4);
	(*nova) = *s.top() * *sMatrix;
	s.pop();
	loadMatrix(nova);
}