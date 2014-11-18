#ifndef __MATRIX_STACK_H__
#define __MATRIX_STACK_H__

//#include <stack>
#include <vector>
#include "Stack.h"

class Matrix;

class MatrixStack
{
	Stack<Matrix*> s;

	static float norm(float *vector, int size);
public:
	MatrixStack();
	~MatrixStack();
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

	static float dotProduct(float *u, float *v, int size);
	static void normalize(float *vector, int size);
	static void crossProduct(float *a, float*b, float* r);
};

#endif