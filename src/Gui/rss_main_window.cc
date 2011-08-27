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
#include <QActionGroup>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <Utilities/console_output.h>
#include <SimulationControl/simulation_control.h>
#include <SzenarioGenerator/szenario_generator.h>
#include <SzenarioGenerator/formation_generator.h>
#include <Wrapper/lua_distribution_generator.h>
#include "../Model/robot_data.h"

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
	delete rss_gl_widget_;
	delete speed_signal_mapper_;
	delete cam_signal_mapper_;
}

void RSSMainWindow::init() {
	open_dialog_ = new OpenProjectDialog(this);
	generator_wizard_ = new GeneratorWizard(this);
	rss_gl_widget_ = new RSSGLWidget(this);
	speed_signal_mapper_ = new QSignalMapper(this);
	cam_signal_mapper_ = new QSignalMapper(this);

	this->setCentralWidget(rss_gl_widget_);

	// action groups
	QActionGroup* action_group = new QActionGroup(this);
	action_group->addAction(ui_.action_free_cam);
	action_group->addAction(ui_.action_follow_swarm_cam);
	action_group->addAction(ui_.action_center_of_gravity_cam);

	// connect signals/slots
	connect(ui_.action_open_project, SIGNAL(triggered()), open_dialog_, SLOT(show()));
	connect(ui_.action_new_project, SIGNAL(triggered()), generator_wizard_, SLOT(show()));
	connect(open_dialog_, SIGNAL(accepted()), this, SLOT(update_simulation()));
	connect(generator_wizard_, SIGNAL(accepted()), this, SLOT(generate_simulation()));
	connect(ui_.action_start_stop_simulation, SIGNAL(triggered()), this, SLOT(toggle_simulation()));
	connect(ui_.action_next_step, SIGNAL(triggered()), this, SLOT(step_simulation()));
	connect(ui_.action_increase_speed, SIGNAL(triggered()), speed_signal_mapper_, SLOT(map()));
	connect(ui_.action_decrease_speed, SIGNAL(triggered()), speed_signal_mapper_, SLOT(map()));
	connect(ui_.action_half_speed, SIGNAL(triggered()), speed_signal_mapper_, SLOT(map()));
	connect(ui_.action_double_speed, SIGNAL(triggered()), speed_signal_mapper_, SLOT(map()));
	connect(ui_.action_free_cam, SIGNAL(triggered()), cam_signal_mapper_, SLOT(map()));
	connect(ui_.action_follow_swarm_cam, SIGNAL(triggered()), cam_signal_mapper_, SLOT(map()));
	connect(ui_.action_center_of_gravity_cam, SIGNAL(triggered()), cam_signal_mapper_, SLOT(map()));
	connect(speed_signal_mapper_, SIGNAL(mapped(int)), this, SLOT(update_simulation_speed(int)));
	connect(cam_signal_mapper_, SIGNAL(mapped(int)), this, SLOT(set_camera_mode(int)));
	connect(rss_gl_widget_, SIGNAL(selected_robot_changed(boost::shared_ptr<RobotData>)), this, SLOT(select_robot(boost::shared_ptr<RobotData>)));

	// map signals
	speed_signal_mapper_->setMapping(ui_.action_increase_speed, INCREASE_SPEED);
	speed_signal_mapper_->setMapping(ui_.action_decrease_speed, DECREASE_SPEED);
	speed_signal_mapper_->setMapping(ui_.action_half_speed, HALF_SPEED);
	speed_signal_mapper_->setMapping(ui_.action_double_speed, DOUBLE_SPEED);
	cam_signal_mapper_->setMapping(ui_.action_free_cam, CAM_FREE);
	cam_signal_mapper_->setMapping(ui_.action_follow_swarm_cam, CAM_FOLLOW);
	cam_signal_mapper_->setMapping(ui_.action_center_of_gravity_cam, CAM_COG);

	// setup icons
	QCommonStyle style;
	ui_.action_new_project->setIcon(style.standardIcon(QStyle::SP_FileIcon));
	ui_.action_open_project->setIcon(style.standardIcon(QStyle::SP_DirOpenIcon));
	ui_.action_quit->setIcon(style.standardIcon(QStyle::SP_DialogCloseButton));
}

void RSSMainWindow::toggle_simulation() {
	boost::shared_ptr<SimulationControl> simulation_control_ = rss_gl_widget_->simulation_control();
	if(!simulation_control_.get())
		return;
	simulation_control_->pause_processing_time(!ui_.action_start_stop_simulation->isChecked());
}

void RSSMainWindow::update_simulation_speed(int op) {
	boost::shared_ptr<SimulationControl> simulation_control_ = rss_gl_widget_->simulation_control();
	if(!simulation_control_.get())
		return;

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


void RSSMainWindow::select_robot(boost::shared_ptr<RobotData> robot_data) {
	if(!robot_data.get()) {
		return;
	}
	ui_.inspector_tree_widget->clear();
	QList<QTreeWidgetItem *> items;

    QTreeWidgetItem *item = new QTreeWidgetItem(ui_.inspector_tree_widget);
    item->setText(0, tr("Position"));
    item->setText(1, QString("(%1, %2, %3)")
    		.arg(robot_data->position()[0])
    		.arg(robot_data->position()[1])
    		.arg(robot_data->position()[2]));

}

void RSSMainWindow::set_camera_mode(int mode) {
	boost::shared_ptr<SimulationRenderer> simulation_renderer = rss_gl_widget_->simulation_renderer();
	simulation_renderer->set_active_cam(mode);
}

void RSSMainWindow::step_simulation() {
	boost::shared_ptr<SimulationControl> simulation_control_ = rss_gl_widget_->simulation_control();
	if(!simulation_control_.get())
		return;

	ui_.action_start_stop_simulation->setChecked(false);
	if(simulation_control_->is_single_step_mode()) {
		simulation_control_->do_single_step();
	} else {
		simulation_control_->enter_single_step_mode();
	}
}

void RSSMainWindow::update_simulation() {
	ProjectData pd = open_dialog_->project_data();

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

void RSSMainWindow::generate_simulation() {
	boost::program_options::variables_map vm = generator_wizard_->generator_data();

	try {
		// init
		ScenarioGenerator generator(vm["seed"].as<unsigned int>());	// set seed
		generator.init(vm);				// init distribution generator

		// files
		generator.set_worldFile(vm["swarmfile"].as<std::string>());
		if (vm.count("robotfile"))
			generator.set_robotFile(vm["robotfile"].as<std::string>());
		else
			generator.set_robotFile(vm["swarmfile"].as<std::string>());
		if (vm.count("obstaclefile"))
			generator.set_obstacleFile(vm["obstaclefile"].as<std::string>());
		else
			generator.set_obstacleFile(vm["swarmfile"].as<std::string>());

		// distribute everything
		generator.distribute();

		// always start coord-system distributer
		generator.distribute_coordsys(vm);

		// sets request handler if requested
		if (vm.count("add-pos-handler"))
			generator.add_play_pos_request_handler();
		if (vm.count("add-vel-handler"))
			generator.add_play_vel_request_handler();
		if (vm.count("add-acc-handler"))
			generator.add_play_acc_request_handler();

		// write to file
		generator.write_to_file();

		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Robots were generated!";
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Please see file: "
																		   << vm["swarmfile"].as<std::string>()
																		   << ".swarm";
	}
	catch (std::exception& e) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << e.what();
		throw;
	}
	catch(...) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "Uncaught unknown exception.";
		throw; //rethrow exception
	}

}
