
#ifndef GUI_H_
#define GUI_H_

#ifndef __linux__
#include <windows.h>
#endif
#include <GL/glut.h>
#include "simulation_control.h"


class GUI{
		public:
			GUI();
			virtual ~GUI();
			virtual void initGUI(int argc, char **argv) = 0;

		private:
			SimulationControl sim_control;
	};

#endif /* GUI_H_ */
