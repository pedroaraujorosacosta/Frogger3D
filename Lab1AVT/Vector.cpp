#include "Vector.h"
#include <iostream>

#include <math.h>

Vector::Vector(float* values, int size)
{
	for (int i = 0; i < size; i++)
		this->v[i] = values[i];
	this->size = size;
}


Vector::Vector(int size)
{
	for (int i = 0; i < size; i++)
		this->v[i] = 0.0f;
	this->size = size;

}

Vector::Vector(const Vector& other) 
{
	for (int i = 0; i < other.size; i++)
		v[i] = other.v[i];
	this->size = other.size;
}

Vector::~Vector() 
{

}

float Vector::norm(float *vector, int size)
{
	float norm = 0;
	for (int i = 0; i < size; i++)
		norm += vector[i] * vector[i];

	return sqrt(norm);
}


void Vector::normalize()
{
	float magnitude = norm(v, size);
	for (int i = 0; i < size; i++)
		v[i] = v[i] / magnitude;
}

void Vector::printVector() 
{
	for (int i = 0; i < this->size; i++) {
		std::cout << this->v[i] << " ";	
	}
}

Vector& Vector::operator=(const Vector &v1)
{
	for (int i = 0; i < v1.size; i++)
		v[i] = v1.v[i];
	return *this;
}

