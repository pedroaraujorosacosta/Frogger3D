#ifndef __VECTOR_H__
#define __VECTOR_H__


class Vector
{
	float norm(float *vector, int size);
	
public:
	float v[4];
	int size;

	Vector(float* values, int size);
	Vector(int size);
	Vector(const Vector& other);
	~Vector();
	void normalize();
	float magnitude();
	void printVector();
	Vector& operator=(const Vector &v1);
	Vector operator-(const Vector &v1);
};
#endif