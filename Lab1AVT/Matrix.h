#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector.h"
#include <vector>


// class Matrix - so funciona p matrizes quadradas.

class Matrix {
public:
	float *m;
	int size;

	Matrix(int size = 4);
	Matrix(float *o, int size);
	Matrix(Matrix &other);
	void printMatrix();
	~Matrix();

	Matrix& operator*=(const Matrix &m1);
	Matrix& operator=(const Matrix &m1);
	Vector operator*(const Vector &v1);
	friend Matrix operator*(Matrix lhs, const Matrix &rhs);
	void transposeMatrix();
	Matrix invertMatrix();
	int sizeofMatrix();
};

#endif 