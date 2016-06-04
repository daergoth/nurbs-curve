#include "NurbsCurve.h"

const GLfloat NurbsCurve::PI = 3.1415926539;
GLint NurbsCurve::ctrlClicked = -1;
GLint NurbsCurve::knotClicked = -1;
GLint NurbsCurve::K = 3;
GLfloat NurbsCurve::maxKnotValue = 1;

std::vector<Vector*> NurbsCurve::ctrlpoints = std::vector<Vector*>();
std::vector<GLfloat> NurbsCurve::weights = std::vector<GLfloat>();
std::vector<GLfloat> NurbsCurve::knots = std::vector<GLfloat>();

NurbsCurve::~NurbsCurve() {
	for (Vector* p : ctrlpoints) {
		delete p;
	}

}

void NurbsCurve::init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, NurbsCurve::SCREEN_WIDTH, 0.f, NurbsCurve::SCREEN_HEIGHT, 0.f, 1.f);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(BASE_POINT_SIZE);
}

GLfloat NurbsCurve::N(GLint i, GLint k, GLfloat u)
{
	if (k == 1) {
		if (u >= knots[i] && u < knots[i+1] && knots[i] < knots[i + 1]) {
			return 1;
		}
		else {
			return 0;
		}
	}
	GLfloat result = 0;

	GLfloat a = (u - knots[i]) / (knots[i + k - 1] - knots[i]);

	if ((knots[i + k - 1] - knots[i]) == 0) {
		a = 0;
	}

	if (a != 0) {
		result += a * N(i, k - 1, u);
	} 

	GLfloat b = (knots[i + k] - u) / (knots[i + k] - knots[i + 1]);

	
	if ((knots[i + k] - knots[i + 1]) == 0) {
		b = 0;
	}

	if (b != 0) {
		result += b * N(i + 1, k - 1, u);
	}

	return result;
}

void NurbsCurve::drawNurbs()
{
	if (ctrlpoints.size() >= 3) {

		glColor3f(0, 1, 0);
		glLineWidth(2);
		glBegin(GL_LINE_STRIP);
		for (GLfloat u = knots[K - 1]; u <= knots[ctrlpoints.size()]; u += 0.01) {
			Vector p({0, 0});
			for (int i = 0; i < ctrlpoints.size();++i) {
				Vector o = *ctrlpoints[i] * N(i, K, u) * weights[i];
				
				p[0] += o[0];
				p[1] += o[1];
			}

			GLfloat sum = 0;
			for (int i = 0; i < ctrlpoints.size(); ++i) {
				sum += N(i, K, u) * weights[i];
			}


			//std::cout << "sum: " << sum << std::endl;

			//std::cout << "n p: " << p[0] / sum << ", " << p[1]/sum  << std::endl;
	
			glVertex2d(p[0]/sum, p[1]/sum);
		}
		glEnd();
	}
}

void NurbsCurve::drawControls()
{
	glLineWidth(1);
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	for (Vector* p : ctrlpoints) {
		glVertex2d((*p)[0], (*p)[1]);
	}
	glEnd();

	glColor3f(1, 0, 0);
	for (int i = 0; i < ctrlpoints.size(); ++i) {
		glPointSize(BASE_POINT_SIZE * weights[i]);
		glBegin(GL_POINTS);

		glVertex2d(ctrlpoints[i]->coords[0], ctrlpoints[i]->coords[1]);

		glEnd();
	}

	if (!knots.empty()) {

		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex2d(140, 20);
		glVertex2d(1140, 20);
		glEnd();


	
		glPointSize(10);
		glBegin(GL_POINTS);
		/*
		for (GLint i = 0; i < knots.size(); ++i) {
			glColor3ub(knotColors[i % 6][0], knotColors[i % 6][1], knotColors[i % 6][2]);
			glVertex2d(140 + 1000 * knots[i], 20);
		}
		*/
		GLint delta_knot = knots.size() - 1;
		GLint current_knot = 0;
		for (GLint i = 0; i < knots.size(); ++i) {
			glColor3ub(knotColors[current_knot % 6][0], knotColors[current_knot % 6][1], knotColors[current_knot % 6][2]);
			glVertex2d(140 + 1000 * (knots[current_knot] / maxKnotValue), 20);

			current_knot += delta_knot;
			if (delta_knot > 0) {
				delta_knot = -(delta_knot - 1);
			} else if (delta_knot < 0) {
				delta_knot = -(delta_knot + 1);
			}
		}

		glEnd();
	}


}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	GLdouble xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	ypos = NurbsCurve::SCREEN_HEIGHT - ypos;

	std::cout << "ctrl_cl: " << NurbsCurve::ctrlClicked << std::endl;
	std::cout << "knot_cl: " << NurbsCurve::knotClicked << std::endl;
	NurbsCurve::ctrlClicked = -1;
	NurbsCurve::knotClicked = -1;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (int i = 0; i < NurbsCurve::ctrlpoints.size(); ++i) {
			if (std::fabs(xpos - NurbsCurve::ctrlpoints[i]->coords[0]) <= NurbsCurve::BASE_POINT_SIZE * NurbsCurve::weights[i]
				&& std::fabs(ypos - NurbsCurve::ctrlpoints[i]->coords[1]) <= NurbsCurve::BASE_POINT_SIZE * NurbsCurve::weights[i]) {
				NurbsCurve::ctrlClicked = i;
				return;
			}
		}

		GLboolean end_knot = false;
		for (GLint i = NurbsCurve::knots.size() - 1; i >= 0; --i) {
			if (std::fabs(xpos - (140 + 1000 * (NurbsCurve::knots[i] / NurbsCurve::maxKnotValue))) <= 10
				&& std::fabs(ypos - 20) <= 10) {
				
				if (i == 0 || i == (NurbsCurve::knots.size() - 1)) {
					end_knot = true;
					std::cout << "endKnot" << std::endl;
					continue;
				}
				else if (NurbsCurve::knots[i] == NurbsCurve::knots[i + 1] && NurbsCurve::knots[i] == NurbsCurve::knots[i - 1]) {
					continue;
				}

				NurbsCurve::knotClicked = i;
				return;
			}

		}
		if (end_knot) {
			return;
		}

		std::cout << "new p: " << xpos << ", " << ypos << std::endl;
		
		NurbsCurve::ctrlpoints.push_back(new Vector({(GLfloat)xpos, (GLfloat)ypos}));
		NurbsCurve::weights.push_back(1.0);

		if (NurbsCurve::ctrlpoints.size() == 3) {
			NurbsCurve::knots.push_back(0);
			NurbsCurve::knots.push_back(0);
			NurbsCurve::knots.push_back(0);
			NurbsCurve::knots.push_back(NurbsCurve::maxKnotValue);
			NurbsCurve::knots.push_back(NurbsCurve::maxKnotValue);
			NurbsCurve::knots.push_back(NurbsCurve::maxKnotValue);
		}
		else if (NurbsCurve::ctrlpoints.size() > 3) {
			NurbsCurve::knots.push_back(NurbsCurve::maxKnotValue);
		}
	}
		
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (NurbsCurve::ctrlClicked != -1) {
		ypos = NurbsCurve::SCREEN_HEIGHT - ypos;

		NurbsCurve::ctrlpoints[NurbsCurve::ctrlClicked]->coords[0] = xpos;
		NurbsCurve::ctrlpoints[NurbsCurve::ctrlClicked]->coords[1] = ypos;
	} else if (NurbsCurve::knotClicked != -1) {
		ypos = NurbsCurve::SCREEN_HEIGHT - ypos;

		GLfloat newKnotVal = (xpos - 140.0) / 1000.0 * NurbsCurve::maxKnotValue;

		if (newKnotVal >= NurbsCurve::knots[NurbsCurve::knotClicked + 1]) {
			newKnotVal = NurbsCurve::knots[NurbsCurve::knotClicked + 1];
		}
		else if (newKnotVal <= NurbsCurve::knots[NurbsCurve::knotClicked - 1]) {
			newKnotVal = NurbsCurve::knots[NurbsCurve::knotClicked - 1];
		}

		NurbsCurve::knots[NurbsCurve::knotClicked] = newKnotVal;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	ypos = NurbsCurve::SCREEN_HEIGHT - ypos;

	for (int i = 0; i < NurbsCurve::ctrlpoints.size(); ++i) {
		if (std::fabs(xpos - NurbsCurve::ctrlpoints[i]->coords[0]) < NurbsCurve::BASE_POINT_SIZE * NurbsCurve::weights[i]
					&& std::fabs(ypos - NurbsCurve::ctrlpoints[i]->coords[1]) < NurbsCurve::BASE_POINT_SIZE * NurbsCurve::weights[i]) {
			NurbsCurve::weights[i] = (NurbsCurve::weights[i] + yoffset * NurbsCurve::DELTA_WEIGHT) <= 1? 1 : NurbsCurve::weights[i] + yoffset * NurbsCurve::DELTA_WEIGHT;
			std::cout << "new w: " << NurbsCurve::weights[i] << std::endl;
			return;
		}
	}
}

void staticKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_KP_ADD ) {
			NurbsCurve::knots.push_back(1);
			NurbsCurve::K++;
		} else if (key == GLFW_KEY_KP_SUBTRACT) {
			if (NurbsCurve::K > 2) { 
				NurbsCurve::knots.pop_back();
				NurbsCurve::K--;

				if (NurbsCurve::knots[NurbsCurve::knots.size() - 1] != 1) {
					NurbsCurve::knots[NurbsCurve::knots.size() - 1] = 1;
				}
				
			}
		}
		else if (key == GLFW_KEY_UP) {
			NurbsCurve::maxKnotValue += NurbsCurve::DELTA_MAXKNOT;
			if (!NurbsCurve::knots.empty()) {
				NurbsCurve::knots[NurbsCurve::knots.size() -1] = NurbsCurve::maxKnotValue;
			}
		} else if (key == GLFW_KEY_DOWN) {
			NurbsCurve::maxKnotValue -= NurbsCurve::DELTA_MAXKNOT;
			if (NurbsCurve::maxKnotValue < 1) {
				NurbsCurve::maxKnotValue = 1;
			}
			if (!NurbsCurve::knots.empty()) {
				for (GLint i = 0; i < NurbsCurve::knots.size(); ++i) {
					if (NurbsCurve::knots[i] > NurbsCurve::maxKnotValue) {
						NurbsCurve::knots[i] = NurbsCurve::maxKnotValue;
					}
				}
			}
		}
	}
}

int main() {
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(NurbsCurve::SCREEN_WIDTH, NurbsCurve::SCREEN_HEIGHT, "NURBS Curve", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, staticKeyCallback);

	/* -- Init -- */
	NurbsCurve::init();
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		/* -- Render -- */
		NurbsCurve::drawControls();
		NurbsCurve::drawNurbs();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	
	/* -- Terminate -- */

	return 0;
}
