/*
 * glut_gui.cc
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "../OpenGL/pg_glut.h"

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

	// initialize simulation control and start simulation
	sim_control->set_visualizer(sim_renderer);
	sim_control->create_new_simulation("<project_file>", 5),
	sim_control->start_simulation();

	// initialize glut and renderer
	PgGLUT::init("Schlauer Schwarmsimulator", argc, argv);
	sim_renderer->init(500,500);

	// set glut callbacks
	PgGLUT::glutDisplayFunc(boost::bind(&SimulationControl::process_simulation, sim_control));
	PgGLUT::glutReshapeFunc(boost::bind(&SimulationRenderer::resize, sim_renderer,_1,_2));
	PgGLUT::glutMouseFunc(boost::bind(&SimulationRenderer::mouse_func,sim_renderer,_1,_2,_3,_4));
	PgGLUT::glutKeyboardFunc(boost::bind(&SimulationRenderer::keyboard_func,sim_renderer,_1,_2,_3));

	// enter main loop (will never return)
	PgGLUT::glutMainLoop();
	return 0;
}
*/
