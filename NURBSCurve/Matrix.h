#pragma once
#include "Vector.h"
#include <iostream>

class Matrix
{
public:
	std::vector<Vector> v;
	Matrix();
	Matrix(std::vector<Vector>&);
	Matrix(const std::initializer_list<std::initializer_list<GLfloat> >&);
	~Matrix();
	Vector operator[](GLint);
	const Vector operator[](GLint) const;
	Matrix operator*(const Matrix &);
	Matrix inhomMulti(const Matrix & arg);
	Vector operator*(const Vector &);
	Matrix operator*(const GLfloat &);

	void destructMultiply(Vector*);

	static Matrix inverse(Matrix &);
private:
	GLint isPivotCorrect();
	void swapRow(GLint, GLint);
};
