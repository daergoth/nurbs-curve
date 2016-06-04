#include "Matrix.h"

Matrix::Matrix()
{
}

Matrix::Matrix(std::vector<Vector>& _v)
{
	v = std::vector<Vector>(_v);
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<GLfloat>>& m)
{
	for (auto it = m.begin(); it != m.end(); ++it) {
		std::vector<GLfloat> tmpFloat;
		for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
			tmpFloat.push_back(*it2);
		}
		v.push_back(Vector(tmpFloat));
	}
}

Matrix::~Matrix()
{
	
}

Vector Matrix::operator[](GLint n) { 
	return v[n]; 
}

const Vector Matrix::operator[](GLint n) const { 
	return v[n]; 
}

Matrix Matrix::operator*(const Matrix & arg)
{	
	std::vector<Vector> tempVect;

	for (GLint col = 0; col < arg.v.size(); ++col) {
		std::vector<GLfloat> tempCol;
		for (GLint row = 0; row < v[0].coords.size(); ++row) {
			GLdouble sum = 0.0;
			for (GLint it = 0; it < v.size(); ++it) {
				sum += v[it][row] * arg.v[col][it];
			}
			tempCol.push_back(sum);
		}
		tempVect.push_back(tempCol);
	}

	return Matrix(tempVect);
}

Matrix Matrix::inhomMulti(const Matrix & arg) {
	std::vector<Vector> tempVect;
	for (GLint row = 0; row < v[0].coords.size(); ++row) {
		std::vector<GLfloat> tempCol;
		for (GLint col = 0; col < arg.v.size(); ++col) {
			GLfloat sum = 0;
			for (GLint it = 0; it < v.size(); ++it) {
				sum += v[it][row] * arg.v[col][it];
			}
			tempCol.push_back(sum);
		}
		tempVect.push_back(tempCol);
	}

	return Matrix(tempVect);
}

Vector Matrix::operator*(const Vector & arg)
{	
  //std::cerr << "matrix op* st" << std::endl;
	std::vector<GLfloat> tempCol;
	
//   std::cerr  << "test " << v.size() << std::endl;
	for (GLint row = 0; row < v[0].coords.size(); ++row) {
		GLfloat sum = 0.0;
		for (GLint it = 0; it < arg.coords.size(); ++it) {
			sum += v[it][row] * arg.coords[it];
		}
		tempCol.push_back(sum);
	}
//    std::cerr << "matrix op* ok" << std::endl;
	return Vector(tempCol);
}

Matrix Matrix::operator*(const GLfloat & r)
{
	Matrix tmp(*this);
	for (std::vector<Vector>::iterator it = tmp.v.begin(); it != tmp.v.end(); ++it) {
		for (GLint t = 0; t < it->coords.size(); ++t) {
		    if ((it)->coords[t] != 0)
			(it)->coords[t] *= r;
		}
	}
	return tmp;
}

void Matrix::destructMultiply(Vector* arg)
{
	GLfloat* tempCol = new GLfloat[v[0].coords.size()];

	for (GLint row = 0; row < v[0].coords.size(); ++row) {
		GLfloat sum = 0.0;
		for (GLint it = 0; it < arg->coords.size(); ++it) {
			sum += v[it][row] * arg->coords[it];
		}
		tempCol[row] = sum;
	}
	
	arg->setNull();
	for (GLint i = 0; i < v[0].coords.size(); ++i) {
		arg->coords[i] = tempCol[i];
	}

	delete[] tempCol;
}

Matrix Matrix::inverse(Matrix & arg)
{
	std::vector<Vector> rightsideV;
	for (GLint s = 0; s < arg.v.size(); ++s) {
		std::vector<GLfloat> temp;
		for (GLint b = 0; b < arg.v.size(); ++b) {
			GLfloat j = 0;
			if (b == s) {
				j = 1;
			}
			temp.push_back(j);
		}
		rightsideV.push_back(Vector(temp));
	}
	
	Matrix rightside = Matrix(rightsideV);


	for (GLint col = 0; col < arg.v.size(); col++) {
		GLint toSwap = arg.v[col].indexOfMaxCoord(col);
		//std::cerr << " ### indexOfMaxCoord: " << toSwap << std::endl;
		if (toSwap != col && arg.v[col][toSwap] > arg.v[col][col]) {
			arg.swapRow(col, toSwap);
			rightside.swapRow(col, toSwap);
		}

		for (GLint row = 0; row < arg.v.size(); row++) {
			if (col == row) continue;

			GLfloat r = arg.v[col][row] / arg.v[col][col];
			//std::cerr << " ### Ratio for Row " << row << ": " << r << std::endl;
			for (GLint it = col; it < arg.v.size(); ++it) {
				arg.v[it][row] -= r * arg.v[it][col];
			}
			for (GLint it = 0; it < arg.v.size(); ++it) {
				rightside.v[it][row] -= r * rightside.v[it][col];
			}
		}

	}

	for (GLint a = 0; a < arg.v.size(); ++a) {
		GLfloat o = arg.v[a][a];
		for (GLint t = 0; t < arg.v.size(); ++t) {
			arg.v[t][a] /= o;
			rightside.v[t][a] /= o;
		}
	}
	return rightside;
}

GLint Matrix::isPivotCorrect()
{
	for (int i = 0; i<v.size(); ++i) {
		if ((v[i])[i] == 0) {
			return i;
		}
	}
	return -1;
}

void Matrix::swapRow(GLint a, GLint b)
{
	for (std::vector<Vector>::iterator it = v.begin(); it != v.end(); ++it) {
		GLfloat tmp = it->coords[a];
		it->coords[a] = it->coords[b];
		it->coords[b] = tmp;
	}
}