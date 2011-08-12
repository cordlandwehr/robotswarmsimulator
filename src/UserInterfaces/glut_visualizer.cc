/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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
		char* argv[] = { (char*)"dummy" }; //dummy value needed since otherwise glutinit crashes (on windows)

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
				simulation_control_.pause_processing_time(!simulation_control_.is_processing_time_paused());
			break;
		case 's':
				// simulate one more step
				if(simulation_control_.is_single_step_mode()) {
					simulation_control_.do_single_step();
				} else {
					simulation_control_.enter_single_step_mode();
				}
			break;
		case 'e':
				// exit single step mode
				simulation_control_.exit_single_step_mode();
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
				simulation_control_.pause_processing_time(!simulation_control_.is_processing_time_paused());
				SimulationRenderer::keyboard_func(key,x,y);
			break;
		case 'd':
				simulation_control_.dump_simulation();
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
