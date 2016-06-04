#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

class Vector
{
public:
	std::vector<GLfloat> coords;

	Vector(const std::vector<GLfloat>&);
	Vector(GLfloat = 0, GLfloat = 0, GLfloat = 0, GLfloat = 1);
	Vector(const std::initializer_list<GLfloat>&);

	~Vector();

	Vector operator+(const Vector &);
	Vector operator-(const Vector &);
	Vector operator*(const GLfloat &);
	Vector operator/(const GLfloat &);
	void destructMultiply(const GLfloat &);

	GLfloat& operator[](const GLint);
	const GLfloat& operator[](const GLint) const;

	GLint indexOfMaxCoord(GLint);
	void setNull();
	
	static GLfloat len(const Vector &);
	static Vector normalize(const Vector);
	static GLfloat len2(const Vector &);
	static GLfloat innerProduct(const Vector &, const Vector &);
	static Vector crossProduct(const Vector &, const Vector &);

	
	friend std::ostream& operator<<(std::ostream& os, const Vector& obj)
	{
	  os << "[";
	  for (GLfloat f : obj.coords)
	    os << f << ", ";
	  os << "]";
	  return os;
	}
};

