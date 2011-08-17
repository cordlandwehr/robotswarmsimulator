/*
 * RSSMainWindow.cc
 *
 *  Created on: 26.07.2011
 *      Author: Sascha Brandt
 */

#include "rss_main_window.h"

#include <QCommonStyle>
#include <QBitmap>
#include <QResource>
#include <QDir>

#include <boost/shared_ptr.hpp>

#include <Utilities/console_output.h>
#include <SimulationControl/simulation_control.h>
#include <Wrapper/lua_distribution_generator.h>

#include "open_project_dialog.h"
#include "generator_wizard.h"
#include "rss_gl_widget.h"

// creates a togglable icon
QIcon standardToggleIcon(const QStyle& style, QStyle::StandardPixmap iconOn, QStyle::StandardPixmap iconOff) {
	QIcon * icon = new QIcon();
	icon->addPixmap(style.standardPixmap(iconOn), QIcon::Normal, QIcon::On);
	icon->addPixmap(style.standardPixmap(iconOff), QIcon::Normal, QIcon::Off);
	return *icon;
}

RSSMainWindow::RSSMainWindow(QWidget *parent) : QMainWindow(parent) {
	ui_.setupUi(this);
}

RSSMainWindow::~RSSMainWindow() {
	delete open_dialog_;
	delete generator_wizard_;
}

void RSSMainWindow::init() {
	open_dialog_ = new OpenProjectDialog(this);
	generator_wizard_ = new GeneratorWizard(this);
	rss_gl_widget_ = new RSSGLWidget(this);
	signal_mapper_ = new QSignalMapper(this);

	this->setCentralWidget(rss_gl_widget_);

	// connect signals/slots
	connect(ui_.action_open_project, SIGNAL(activated()), open_dialog_, SLOT(show()));
	connect(ui_.action_new_project, SIGNAL(activated()), generator_wizard_, SLOT(show()));
	connect(open_dialog_, SIGNAL(accepted()), this, SLOT(updateSimulation()));
	connect(ui_.action_start_stop_simulation, SIGNAL(activated()), this, SLOT(toggleSimulation()));
	connect(ui_.action_next_step, SIGNAL(activated()), this, SLOT(stepSimulation()));
	connect(ui_.action_increase_speed, SIGNAL(activated()), signal_mapper_, SLOT(map()));
	connect(ui_.action_decrease_speed, SIGNAL(activated()), signal_mapper_, SLOT(map()));
	connect(ui_.action_half_speed, SIGNAL(activated()), signal_mapper_, SLOT(map()));
	connect(ui_.action_double_speed, SIGNAL(activated()), signal_mapper_, SLOT(map()));

	// map signals
	signal_mapper_->setMapping(ui_.action_increase_speed, INCREASE_SPEED);
	signal_mapper_->setMapping(ui_.action_decrease_speed, DECREASE_SPEED);
	signal_mapper_->setMapping(ui_.action_half_speed, HALF_SPEED);
	signal_mapper_->setMapping(ui_.action_double_speed, DOUBLE_SPEED);

	// setup icons
	QCommonStyle style;
	ui_.action_new_project->setIcon(style.standardIcon(QStyle::SP_FileIcon));
	ui_.action_open_project->setIcon(style.standardIcon(QStyle::SP_DirOpenIcon));
	ui_.action_quit->setIcon(style.standardIcon(QStyle::SP_DialogCloseButton));
}

void RSSMainWindow::toggleSimulation() {
	boost::shared_ptr<SimulationControl> simulation_control_ = rss_gl_widget_->simulation_control();
	simulation_control_->pause_processing_time(!ui_.action_start_stop_simulation->isChecked());
}

void RSSMainWindow::updateSimulationSpeed(int op) {
	boost::shared_ptr<SimulationControl> simulation_control_ = rss_gl_widget_->simulation_control();
	switch(op) {
	case INCREASE_SPEED:
		simulation_control_->increase_processing_time_linearly();
		break;
	case DECREASE_SPEED:
		simulation_control_->decrease_processing_time_linearly();
		break;
	case DOUBLE_SPEED:
		simulation_control_->increase_processing_time_exp();
		break;
	case HALF_SPEED:
		simulation_control_->decrease_processing_time_exp();
		break;
	}
}

void RSSMainWindow::stepSimulation() {
	boost::shared_ptr<SimulationControl> simulation_control_ = rss_gl_widget_->simulation_control();

	ui_.action_start_stop_simulation->setChecked(false);
	if(simulation_control_->is_single_step_mode()) {
		simulation_control_->do_single_step();
	} else {
		simulation_control_->enter_single_step_mode();
	}
}

void RSSMainWindow::updateSimulation() {
	ProjectData pd = open_dialog_->projectData();

	std::string tmpProjectFile = pd.project_file;

	// deletes ".swarm" from end of file, if used
	if (tmpProjectFile.rfind(".swarm")!=std::string::npos)
		tmpProjectFile.erase (tmpProjectFile.rfind(".swarm"),6);

	bool run_until_no_multiplicity = pd.run_until_no_multiplicity;

	// initializes the lua random number generator
	if(pd.luaseed > 0) {
		LuaWrapper::lua_generator_set_seed(pd.luaseed);
	}

	// checks iff statistics shall be created
	bool create_statistics = !pd.dry;

	// create simulation kernel
	boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
	if(pd.steps > 0) {
		std::cout <<  "creating simulation with limited steps" << std::endl;
		sim_control->create_new_simulation(tmpProjectFile,
										   pd.history_length,
										   pd.output,
										   create_statistics,
										   true,
										   pd.steps,
										   run_until_no_multiplicity);
	} else {
		std::cout <<  "creating simulation without limited steps" << std::endl;
		sim_control->create_new_simulation(tmpProjectFile,
										   pd.history_length,
										   pd.output,
										   create_statistics,
										   false,
										   0,
										   run_until_no_multiplicity);
	}

	rss_gl_widget_->set_simulation_control(sim_control);
}
