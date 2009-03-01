/*
 * glut_visualizer.cc
 *
 *  Created on: 21.02.2009
 *      Author: peter
 */

#include "glut_visualizer.h"

#include <cmath>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include <OpenGL/glut_headers.h>
#include <OpenGL/pg_glut.h>


namespace {
	// GLUT timer callback that tries to achieve the requested framerate
	void timer_callback(int ms) {
		glutPostRedisplay();
		glutTimerFunc(ms, &timer_callback, ms);
	}
}

GlutVisualizer::GlutVisualizer(SimulationControl& simulation_control, unsigned int fps)
: SimulationRenderer(), simulation_control_(simulation_control),
  fps_(fps), initialized_(false) {
}

void GlutVisualizer::glutMainLoop() {
	::glutMainLoop();
}

void GlutVisualizer::init() {
	if (!initialized_) {
		int spf = static_cast<int>(std::floor(1./fps_ + 0.5)); // spf = second per frame ;-)

		// dummy variables for GLUT's init method
		int argc = 1;
		char* argv[] = {"dummy"}; //dummy value needed since otherwise glutinit crashes (on windows)

		// initialize/configure glut
		PgGLUT::init("Robot Swarm Simulator", argc, argv);
		PgGLUT::glutDisplayFunc(boost::bind(&SimulationControl::process_simulation, &simulation_control_));
		PgGLUT::glutSpecialFunc(boost::bind(&GlutVisualizer::keyboard_special_func, this, _1, _2, _3));
		PgGLUT::glutKeyboardFunc(boost::bind(&GlutVisualizer::keyboard_func, this, _1, _2, _3));
		PgGLUT::glutMouseFunc(boost::bind(&SimulationRenderer::mouse_func, this, _1, _2, _3, _4));
		PgGLUT::glutReshapeFunc(boost::bind(&SimulationRenderer::resize, this, _1, _2));
		PgGLUT::glutTimerFunc(spf, &timer_callback, 0);
		PgGLUT::glutMotionFunc( boost::bind(&SimulationRenderer::mouse_motion_func, this, _1, _2));

		SimulationRenderer::init();
		initialized_ = true;
	}
}




void GlutVisualizer::keyboard_func(unsigned char key, int x, int y){
	switch(key){
		case 'q':
				simulation_control_.terminate_simulation();
				exit(0);
			break;
		case ' ':
				simulation_control_.pause_processing_time();
			break;
		case '+':
				simulation_control_.increase_processing_time_linearly();
			break;
		case '-':
				simulation_control_.decrease_processing_time_linearly();
			break;
		case '*':
				simulation_control_.increase_processing_time_exp();
			break;
		case '/':
				simulation_control_.decrease_processing_time_exp();
			break;
		case 'h':
				simulation_control_.pause_processing_time();
				SimulationRenderer::keyboard_func(key,x,y);
			break;
		default:
				SimulationRenderer::keyboard_func(key,x,y);
			break;
	}
}


void GlutVisualizer::keyboard_special_func(int key, int x, int y){
	switch(key){

		default:
				SimulationRenderer::keyboard_special_func(key,x,y);
			break;
	}

}
