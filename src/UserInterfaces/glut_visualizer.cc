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

#include <Visualisation/follow_swarm_camera.h>


namespace {
	// GLUT timer callback that tries to achieve the requested framerate
	void timer_callback(int ms) {
		glutPostRedisplay();
		glutTimerFunc(ms, &timer_callback, ms);
	}
}


// TODO(peter) SimulationRenderer should not need to be given a camera. Instead, it should load instances of all
//             implemented cameras and give the user the possibility to switch between them.
//             --> change intializer list of this constructor as soon as the above is implemented
GlutVisualizer::GlutVisualizer(SimulationControl& simulation_control, unsigned int fps)
: SimulationRenderer(boost::shared_ptr<Camera>(new FollowSwarmCamera())), simulation_control_(simulation_control),
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
		PgGLUT::glutKeyboardFunc(boost::bind(&GlutVisualizer::keyboard_func, this, _1, _2, _3));
		PgGLUT::glutMouseFunc(boost::bind(&GlutVisualizer::mouse_func, this, _1, _2, _3, _4));
		PgGLUT::glutReshapeFunc(boost::bind(&GlutVisualizer::resize, this, _1, _2));
		PgGLUT::glutTimerFunc(spf, &timer_callback, 0);

		SimulationRenderer::init();
		initialized_ = true;
	}
}
