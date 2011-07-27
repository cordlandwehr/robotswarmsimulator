/*
 * robot_swarm_simulator_gui.cc
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */

/**
 * \file robot_swarm_simulator_gui.cc
 * \brief Main program used to load a project file, setup the simulation and start it.
 */

#include <QtGui/QApplication>
#include <Gui/RSSMainWindow.h>

int main(int argc, char** argv) {
	QApplication a(argc, argv);
	Q_INIT_RESOURCE(qt_resources);

	RSSMainWindow w;

	w.show();
	return a.exec();
}
