/*
 * glut_gui.h
 *
 *  Created on: 20.01.2009
 *      Author: Alexander
 */
#include "gui.h";
#ifndef GLUT_GUI_H_
#define GLUT_GUI_H_

class GlutGUI: public virtual GUI {
		public:
			GlutGUI();
			virtual ~GlutGUI();
			virtual void initGUI(int argc, char **argv);
	};

#endif /* GLUT_GUI_H_ */
