#include "Matrix.h"
#include <math.h>
#include <iostream>

Matrix::Matrix(int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			m.push_back(0.0f);
}

Matrix::Matrix(float *o, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			m.push_back(o[j + 4 * i]);
}

void Matrix::printMatrix(){
	int size = sqrt((int)m.size());
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			std::cout << this->m[j + 4 * i] << " ";
		std::cout << std::endl;
	}
}

int Matrix::sizeofMatrix() {
	return m.size();
}

Matrix::Matrix(Matrix &other)
{
	for (int i = 0; i < other.m.size(); i++)
		m.push_back(other.m[i]);
}


Vector Matrix::operator*(const Vector &v1)
{

	int size1 = sqrt(m.size());
	int size2 = sqrt(m.size());
	Matrix copy(*this);
	Vector v2(v1.size);

	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < size2; j++)
		{
			v2.v[i] += copy.m[j + size1 * i] * v1.v[j];
		}
	}
	return v2;
}

Matrix& Matrix::operator*=(const Matrix &m1)
{
	int size1 = sqrt(m.size());
	int size2 = sqrt(m1.m.size());
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
	m.clear();
	for (int i = 0; i < m1.m.size(); i++)
		m.push_back(m1.m[i]);
	return *this;
}

/*Matrix Matrix::multMatrix(Matrix &m1, Matrix &m2)
{
	int size1 = sqrt(m1.m.size());
	int size2 = sqrt(m2.m.size());

	if (size1 == size2) {
		for (int i = 0; i < size1; i++)
		{
			for (int j = 0; j < size2; j++)
			{
				m[j + size1 * i] = 0;
				for (int k = 0; k < size1; k++)
				{
					this->m[j + size1 * i] += m1.m[k + size1 * i] * m2.m[j + size2 * k];
				}
			}
		}
	}
	return *this;
}*/

void Matrix::transposeMatrix()
{

}

void Matrix::invertMatrix()
{ 

}