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

#include "../OpenGL/GLUTHeaders.h"
//#include "../OpenGL/PgGLUT.h"

#include "../SimulationControl/simulation_control.h"
#include "../Visualisation/simulation_renderer.h"
#include "../Visualisation/camera.h"


/*
// TODO: -move to init or something
 * 		 -set up threads somewhere else
 *
 *
int main(int argc, char** argv) {
	//ties GLUT functions to SimulationControl and SimulationRenderer functions

	boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
	boost::shared_ptr<Camera> camera(new Camera());
	boost::shared_ptr<SimulationRenderer> sim_renderer(new SimulationRenderer(camera));

	sim_control->set_visualizer(sim_renderer);

	sim_renderer->init(500,500);


	PgGLUT::init("Schlauer Schwarmsimulator", argc, argv);
	PgGLUT::glutDisplayFunc(boost::bind(&SimulationControl::process_simulation, sim_control)); // use boost::bind for class-member callbacks
	PgGLUT::glutReshapeFunc(boost::bind(&SimulationRenderer::resize, sim_renderer,_1,_2));
	PgGLUT::glutMouseFunc(boost::bind(&SimulationRenderer::mouse_func,sim_renderer,_1,_2,_3,_4));
	PgGLUT::glutKeyboardFunc(boost::bind(&SimulationRenderer::keyboard_func,sim_renderer,_1,_2,_3));
	PgGLUT::glutMainLoop();

	return 0;
}
*/
