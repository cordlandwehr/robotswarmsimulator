/*
 * SampleGUI.cc
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */


#include <ctime>
#include <cmath>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "../OpenGL/glut_headers.h"
#include "../OpenGL/pg_glut.h"

#include "SampleGUI.h"

// forward declaration
void reshapeFractal(int width, int height);

// main function
int main(int argc, char** argv) {
	boost::shared_ptr<SampleControl> sample_control(new SampleControl());

	PgGLUT::init("NYSS - Not Yet a Swarm Simulator", argc, argv);
	PgGLUT::glutDisplayFunc(boost::bind(&SampleControl::process_simulation, sample_control)); // use boost::bind for class-member callbacks
	PgGLUT::glutReshapeFunc(&reshapeFractal);                                                 // normal glut syntax for global callback methods
	PgGLUT::glutMainLoop();

	return 0;
}

// class-member callback
void SampleControl::process_simulation() {
	glClearColor(0., 0., 0., 0.);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glTranslated(sin((double)clock()/CLOCKS_PER_SEC), cos((double)clock()/CLOCKS_PER_SEC), 0.);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glFlush();
	glutPostRedisplay();
}

// global method callback
void reshapeFractal(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.5, 2.5, -2.5, 2.5, 1., -1.);
    glMatrixMode(GL_MODELVIEW);
}
