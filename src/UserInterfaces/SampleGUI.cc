/*
 * SampleGUI.cc
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */


#include <ctime>
#include <cmath>

#include <iostream>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "../OpenGL/glut_headers.h"
#include "../OpenGL/pg_glut.h"

#include "../SimulationControl/simulation_control.h"
#include "../Visualisation/simulation_renderer.h"
#include "../Visualisation/follow_swarm_camera.h"


// forward declaration
void timerCB(int millisec);


int main(int argc, char** argv) {
	boost::shared_ptr<SimulationControl> control(new SimulationControl());
	boost::shared_ptr<Camera> camera(new FollowSwarmCamera());
	boost::shared_ptr<SimulationRenderer> visualizer(new SimulationRenderer(camera));

	// register glut callbacks
	PgGLUT::init("NYSS - Not Yet a Swarm Simulator", argc, argv);
    PgGLUT::glutDisplayFunc(boost::bind(&SimulationControl::process_simulation, control));
    PgGLUT::glutKeyboardFunc(boost::bind(&SimulationRenderer::keyboard_func, visualizer, _1, _2, _3));
    PgGLUT::glutMouseFunc(boost::bind(&SimulationRenderer::mouse_func, visualizer, _1, _2, _3, _4));
    PgGLUT::glutReshapeFunc(boost::bind(&SimulationRenderer::resize, visualizer, _1, _2));
    PgGLUT::glutTimerFunc(50, timerCB, 50);

	std::cout << "Initializing Visualization" << std::endl;
	visualizer->init();
	control->set_visualizer(visualizer);

	std::cout << "Creating new Simulation" << std::endl;
	control->create_new_simulation("src/Tests/TestData/garbled_projectfile_a", 25);

	std::cout << "Starting Simulation Thread" << std::endl;
	control->start_simulation();

	PgGLUT::glutMainLoop();
	return 0;
}

void timerCB(int millisec) {
	glutTimerFunc(millisec, timerCB, millisec);
	glutPostRedisplay();
}
