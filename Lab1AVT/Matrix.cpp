#include "Matrix.h"
#include <math.h>
#include <iostream>

Matrix::Matrix(int size)
{
	this->size = size;
	m = new float[size * size];
}

Matrix::Matrix(float *o, int size)
{
	this->size = size;
	m = new float[size * size];
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			m[j + size * i] = o[j + size * i];
}


Matrix::~Matrix()
{
	delete[] m;
}

void Matrix::printMatrix()
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			std::cout << m[j + size * i] << " ";
		std::cout << std::endl;
	}
}

int Matrix::sizeofMatrix() 
{
	return size;
}

Matrix::Matrix(Matrix &other)
{
	this->size = other.size;
	m = new float[size * size];
	for (int i = 0; i < other.size * other.size; i++)
		m[i] = other.m[i];
}


Vector Matrix::operator*(const Vector &v1)
{
	Vector v2(v1.size);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			v2.v[i] += this->m[j + size * i] * v1.v[j];
		}
	}
	return v2;
}

Matrix& Matrix::operator*=(const Matrix &m1)
{
	int size1 = size;
	int size2 = m1.size;
	Matrix copy(*this);

	if (size1 == size2) {
		for (int i = 0; i < size1; i++)
		{
			for (int j = 0; j < size2; j++)
			{
				m[j + size1 * i] = 0.0f;
				for (int k = 0; k < size1; k++)
				{
					this->m[j + size1 * i] += copy.m[k + size1 * i] * m1.m[j + size2 * k];
				}
			}
		}
	}
	return *this;
}

Matrix operator*(Matrix lhs, const Matrix &rhs)
{
	return lhs *= rhs;
}

Matrix& Matrix::operator=(const Matrix &m1)
{
	for (int i = 0; i < size * size; i++)
		m[i] = m1.m[i];
	return *this;
}

void Matrix::transposeMatrix()
{

}

Matrix Matrix::invertMatrix()
{ 
	float det, invDet;
	float mMat3x3[9];

	for (int i = 0; i < 3; i++)
		mMat3x3[i] = m[i];
	for (int i = 4; i < 7; i++)
		mMat3x3[i - 1] = m[i];
	for (int i = 8; i < 11; i++)
		mMat3x3[i - 2] = m[i];

	delete[] m;
	m = new float[9];

	det = mMat3x3[0] * (mMat3x3[4] * mMat3x3[8] - mMat3x3[5] * mMat3x3[7]) +
		mMat3x3[1] * (mMat3x3[5] * mMat3x3[6] - mMat3x3[8] * mMat3x3[3]) +
		mMat3x3[2] * (mMat3x3[3] * mMat3x3[7] - mMat3x3[4] * mMat3x3[6]);
	invDet = 1.0f / det;

	m[0] = (mMat3x3[4] * mMat3x3[8] - mMat3x3[5] * mMat3x3[7]) * invDet;
	m[1] = (mMat3x3[5] * mMat3x3[6] - mMat3x3[8] * mMat3x3[3]) * invDet;
	m[2] = (mMat3x3[3] * mMat3x3[7] - mMat3x3[4] * mMat3x3[6]) * invDet;
	m[3] = (mMat3x3[2] * mMat3x3[7] - mMat3x3[1] * mMat3x3[8]) * invDet;
	m[4] = (mMat3x3[0] * mMat3x3[8] - mMat3x3[2] * mMat3x3[6]) * invDet;
	m[5] = (mMat3x3[1] * mMat3x3[6] - mMat3x3[7] * mMat3x3[0]) * invDet;
	m[6] = (mMat3x3[1] * mMat3x3[5] - mMat3x3[4] * mMat3x3[2]) * invDet;
	m[7] = (mMat3x3[2] * mMat3x3[3] - mMat3x3[0] * mMat3x3[5]) * invDet;
	m[8] = (mMat3x3[0] * mMat3x3[4] - mMat3x3[3] * mMat3x3[1]) * invDet;

	return *this;
}