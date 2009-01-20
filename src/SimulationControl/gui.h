
#ifndef GUI_H_
#define GUI_H_

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
