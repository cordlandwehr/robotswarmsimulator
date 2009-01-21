/*
 * SampleGUI.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef SAMPLEGUI_H_
#define SAMPLEGUI_H_

#include "../SimulationControl/simulation_control.h"

// I inherit here from SimulationControl only to make clear that SimulationControl::process_simulation will be glut's
// display callback later on. For this test, this inheritance is irrelevant.
class SampleControl : public SimulationControl {
public:
	void process_simulation();
};

#endif /* SAMPLEGUI_H_ */
