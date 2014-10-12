#ifndef __STACK_H__
#define __STACK_H__

#include <stack>

class Matrix;

class Stack 
{
	std::stack<Matrix*> s;
	float dotProduct(float *u, float *v, int size);
	float norm(float *vector, int size);
	void normalize(float *vector, int size);
	void crossProduct(float *a, float*b, float* r);
public:
	Stack();
	~Stack();
	void loadIdentity();
	void loadMatrix(Matrix* matrix);
	void cleanStack();
	void push();
	void pop();
	void printStack();
	void translateMatrix(float tx, float ty, float tz);
	void scaleMatrix(float sx, float sy, float sz);
	void rotateMatrix(float x, float y, float z, float alpha);
	void perspective(float left, float right, float bottom, float top,
		float nearPlane, float farPlane);
	void orthogonal(float left, float right, float bottom, float top,
		float nearPlane, float farPlane);
	void lookAt(float eyex, float eyey, float eyez,
		float atx, float aty, float atz,
		float upx, float upy, float upz);
	void lookAt(float *right, float *up, float *eye, float *lookPoint);
	Matrix* getTop();
};

#endif