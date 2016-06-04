#pragma once
#include "Matrix.h"
#include "GLFW\glfw3.h"

class NurbsCurve {

public:
	constexpr static GLint SCREEN_WIDTH = 1280;
	constexpr static GLint SCREEN_HEIGHT = 720;
	constexpr static GLfloat BASE_POINT_SIZE = 3.0;
	constexpr static GLfloat DELTA_WEIGHT = .25;
	constexpr static GLfloat DELTA_MAXKNOT = .25;
	constexpr static GLint knotColors[][3] = {
		{212, 0, 5},
		{255, 86, 0},
		{252, 208, 48},
		{63, 161, 0},
		{12, 46, 123},
		{96, 7, 161}
	};
	
	static const GLfloat PI;

	static GLint ctrlClicked;
	static GLint knotClicked;
	static GLint K;
	static GLfloat maxKnotValue;

	static std::vector<Vector*> ctrlpoints;
	static std::vector<GLfloat> weights;
	static std::vector<GLfloat> knots;

	~NurbsCurve();

	GLfloat static N(GLint i, GLint k, GLfloat u);

	void static init();
	void static drawNurbs();
	void static drawControls();
	
};