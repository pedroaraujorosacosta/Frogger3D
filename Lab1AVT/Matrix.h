#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector.h"
#include <vector>


// class Matrix - so funciona p matrizes quadradas.

class Matrix {
public:
	//std::vector<float> m;
	float *m;
	int size;

	Matrix(int size = 4);
	Matrix(float *o, int size);
	Matrix(Matrix &other);
	void printMatrix();

	Matrix& operator*=(const Matrix &m1);
	Matrix& operator=(const Matrix &m1);
	Vector operator*(const Vector &v1);
	friend Matrix operator*(Matrix lhs, const Matrix &rhs);
	void transposeMatrix();
	Matrix invertMatrix();
	//Matrix multMatrix(Matrix &m1, Matrix &m2);
	int sizeofMatrix();
};

#endif 