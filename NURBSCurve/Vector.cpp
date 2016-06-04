#include "Vector.h"



Vector::Vector(const std::vector<GLfloat>& _crds)
{
	coords = std::vector<GLfloat>(_crds);
}

Vector::Vector(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _h)
{
	coords.push_back(_x);
	coords.push_back(_y);
	coords.push_back(_z);
	coords.push_back(_h);
}

Vector::Vector(const std::initializer_list<GLfloat>& coordArray)
{
	for (std::initializer_list<GLfloat>::const_iterator it = coordArray.begin(); it != coordArray.end(); ++it) {
		coords.push_back(*it);
	}
}

Vector::~Vector()
{
	
}

GLfloat Vector::len(const Vector & vector)
{
	GLfloat sum = 0;
	for (std::vector<GLfloat>::const_iterator it = vector.coords.begin(); it != vector.coords.end(); ++it) {
		sum += std::pow(*it,2);
	}
	return std::sqrt(sum);
}

Vector Vector::normalize(const Vector vector)
{	
	GLfloat len = Vector::len(vector);
	return Vector(vector[0]/len, vector[1]/len, vector[2]/len, 1);
}

GLfloat Vector::len2(const Vector & vector)
{
	GLfloat sum = 0;
	for (std::vector<GLfloat>::const_iterator it = vector.coords.begin(); it != vector.coords.end(); ++it) {
		sum += std::pow(*it, 2);
	}
	return sum;
}

Vector Vector::operator+(const Vector & arg)
{
	Vector tmp(*this);
	for (GLint i = 0; i < 3; ++i) {
		tmp.coords[i] += arg.coords[i];
	}
	return tmp;
}

Vector Vector::operator-(const Vector & arg)
{
	Vector tmp(*this);
	for (GLint i = 0; i < 3; ++i) {
		tmp.coords[i] -= arg.coords[i];
	}
	return tmp;
}

Vector Vector::operator*(const GLfloat & r)
{	
	Vector tmp(*this);
	for (GLint i = 0; i < tmp.coords.size(); ++i) {
		tmp.coords[i] *= r;
	}
	return tmp;
}

Vector Vector::operator/(const GLfloat & r)
{
	Vector tmp(*this);
	tmp = tmp * (GLfloat(1) / r);
	return tmp;
}

void Vector::destructMultiply(const GLfloat & r)
{
	for (GLint i = 0; i < 3; ++i) {
		coords[i] *= r;
	}
}

GLfloat& Vector::operator[](const GLint n) 
{ 
	return coords[n]; 
}
const GLfloat& Vector::operator[](const GLint n) const 
{ 
	return coords[n]; 
}

GLint Vector::indexOfMaxCoord(GLint after)
{
	GLint maxIndex = 0;
	for (GLint i = after; i < coords.size(); ++i) {
		if (coords[i] > coords[maxIndex])
			maxIndex = i;
	}
	return maxIndex;
}

void Vector::setNull()
{
	for (GLint i = 0; i < coords.size() - 1; ++i) {
		coords[i] = 0;
	}
}

GLfloat Vector::innerProduct(const Vector & arg1, const Vector & arg2)
{
	GLfloat r = 0;
	for (GLint i = 0; i < arg1.coords.size()-1; ++i) {
		r += arg1.coords[i] * arg2.coords[i];
	}
	return r;
}

Vector Vector::crossProduct(const Vector & arg1, const Vector & arg2)
{
	return Vector(arg1[1] * arg2[2] - arg1[2] * arg2[1], arg1[2] * arg2[0] - arg1[0] * arg2[2], arg1[0] * arg2[1] - arg1[1] * arg2[0], 1);
}
