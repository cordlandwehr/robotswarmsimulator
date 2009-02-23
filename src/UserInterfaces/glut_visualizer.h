/*
 * glut_visualizer.h
 *
 *  Created on: 21.02.2009
 *      Author: peter
 */

#ifndef GLUT_VISUALIZER_H_
#define GLUT_VISUALIZER_H_

#include <SimulationControl/simulation_control.h>
#include <Visualisation/simulation_renderer.h>


/**
 * \brief Visualizes the simulation process using GLUT.
 */
class GlutVisualizer : public SimulationRenderer {
public:
	/**
	 * \brief Creates a GLUT visualizer that will render the simulation in a GLUT window.
	 * \param simulation_control Used to register it's process_simulation method as GLUT's display method.
	 * \param fps Requested frames per second value for visualization (defaults to 20).
	 */
	GlutVisualizer(SimulationControl& simulation_control, unsigned int fps = 20);

	/**
	 * \brief Simple wrapper that enters GLUT's main loop.
	 *
	 * Note that this method will never return (unless you're using extend versions of GLUT and their 'exit-main-loop'
	 * calls).
	 */
	static void glutMainLoop();

	void keyboard_func(unsigned char key, int x, int y);

	void keyboard_special_func(int key, int x, int y);

	/**
	 * \brief Setup of GLUT and call to init method of SimulationRenderer.
	 *
	 * Note that calling this method more than once has no effect.
	 */
	void init();

private:
	SimulationControl& simulation_control_;
	const unsigned int fps_;
	bool initialized_;
};

#endif /* GLUT_VISUALIZER_H_ */
