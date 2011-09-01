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
#include "../Model/world_information.h"
#include "../Model/message_identifier.h"
#include "../Model/message.h"
#include "../Model/robot_identifier.h"
#include "../Model/robot_data.h"

#include "open_project_dialog.h"
#include "generator_wizard.h"
#include "rss_gl_widget.h"


Q_DECLARE_METATYPE(boost::shared_ptr<Identifier>);
Q_DECLARE_METATYPE(boost::shared_ptr<MessageIdentifier>);
Q_DECLARE_METATYPE(boost::shared_ptr<EdgeIdentifier>);
Q_DECLARE_METATYPE(boost::shared_ptr<RobotIdentifier>);

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
	delete help_dialog_;
	delete about_dialog_;
}

void RSSMainWindow::init() {
	open_dialog_ = new OpenProjectDialog(this);
	generator_wizard_ = new GeneratorWizard(this);
	rss_gl_widget_ = new RSSGLWidget(this);
	speed_signal_mapper_ = new QSignalMapper(this);
	cam_signal_mapper_ = new QSignalMapper(this);
	help_dialog_ = new QDialog(this);
	about_dialog_ = new QDialog(this);

	Ui::HelpDialog help_dialog;
	help_dialog.setupUi(help_dialog_);

	Ui::AboutDialog about_dialog;
	about_dialog.setupUi(about_dialog_);

	this->setCentralWidget(rss_gl_widget_);

	// action groups
	QActionGroup* action_group = new QActionGroup(this);
	action_group->addAction(ui_.action_free_cam);
	action_group->addAction(ui_.action_follow_swarm_cam);
	action_group->addAction(ui_.action_center_of_gravity_cam);

	// connect signals/slots
	connect(ui_.action_open_project, SIGNAL(triggered()), open_dialog_, SLOT(show()));
	connect(ui_.action_new_project, SIGNAL(triggered()), generator_wizard_, SLOT(show()));
	connect(ui_.action_about, SIGNAL(triggered()), about_dialog_, SLOT(show()));
	connect(ui_.action_help, SIGNAL(triggered()), help_dialog_, SLOT(show()));
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
	connect(rss_gl_widget_, SIGNAL(selected_object_changed(boost::shared_ptr<Identifier>)), this, SLOT(selected_object_changed(boost::shared_ptr<Identifier>)));
	connect(ui_.inspector_tree_widget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(tree_selection_changed(QTreeWidgetItem*,int)));

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

	startTimer(500);
}

void RSSMainWindow::toggle_simulation() {
	boost::shared_ptr<SimulationControl> simulation_control_ = rss_gl_widget_->simulation_control();
	if(!simulation_control_.get())
		return;
	simulation_control_->exit_single_step_mode();
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


std::string boost_any_to_string(boost::any * data) {
	std::stringstream ss;
	if(std::string *s = boost::any_cast<std::string>(data)) {
		ss << *s;
	} else if(double *d = boost::any_cast<double>(data)) {
		ss << *d;
	} else if(bool *b = boost::any_cast<bool>(data)) {
		ss << *b ? "true" : "false";
	} else {
		ss << "<unknown type>";
	}
	return ss.str();
}


void RSSMainWindow::timerEvent(QTimerEvent * event) {
	update_selected_object();
}

void RSSMainWindow::selected_object_changed(boost::shared_ptr<Identifier> id) {

	if(!id.get()) {
		return;
	}
	selection_id_ = id;
	ui_.inspector_tree_widget->clear();

	QString type = tr("Object");
	QVariant id_data;

    boost::shared_ptr<WorldInformation> world_info = rss_gl_widget_->simulation_renderer()->world_info();

    boost::shared_ptr<WorldObject> data;
	boost::shared_ptr<RobotIdentifier> r_id = boost::dynamic_pointer_cast<RobotIdentifier>(id);
	boost::shared_ptr<MessageIdentifier> m_id = boost::dynamic_pointer_cast<MessageIdentifier>(id);
	boost::shared_ptr<EdgeIdentifier> e_id = boost::dynamic_pointer_cast<EdgeIdentifier>(id);
	if(r_id.get() && world_info->robot_data().size() > r_id->id() ) {
		data = world_info->get_according_robot_data_ptr(r_id);
		type = tr("Robot");
		id_data = QVariant::fromValue(r_id);
	} else if(m_id.get() && world_info->messages().find(m_id->id()) != world_info->messages().end() ) {
		data = world_info->get_according_message(m_id);
		type = tr("Message");
		id_data = QVariant::fromValue(m_id);
	} else if(e_id.get() && world_info->edges().find(e_id->id()) != world_info->edges().end() ) {
		data = world_info->get_according_edge(e_id);
		type = tr("Edge");
		id_data = QVariant::fromValue(e_id);
	} else {
		return;
	}

	QList<QTreeWidgetItem *> items;

	// id
	QTreeWidgetItem * item = new QTreeWidgetItem(ui_.inspector_tree_widget);
	item->setText(0, type);
	item->setText(1, QString("%1").arg(id->id()));
	item->setData(0,Qt::UserRole,id_data);


	// Position
    item = new QTreeWidgetItem(ui_.inspector_tree_widget);
    item->setText(0, tr("Position"));
    item->setText(1, QString("(%1, %2, %3)")
    		.arg(data->position()[0])
    		.arg(data->position()[1])
    		.arg(data->position()[2]));

	// Marker information
    QTreeWidgetItem * marker_info_item = new QTreeWidgetItem(ui_.inspector_tree_widget);
    marker_info_item->setText(0, tr("Marker Information"));
	marker_info_item->setExpanded(true);

    std::vector<std::string> keys = data->marker_information().get_keys();
    std::vector<std::string>::const_iterator it;
    for( it = keys.begin(); it != keys.end(); ++it ) {
    	boost::any m_data = data->marker_information().get_data(*it);
    	item = new QTreeWidgetItem(marker_info_item);
		item->setText(0, QString::fromStdString(*it));
		item->setText(1, QString::fromStdString(boost_any_to_string(&m_data)));
    }

    if(r_id.get()) {
    	boost::shared_ptr<RobotData> robot_data = boost::dynamic_pointer_cast<RobotData>(data);

		// Edges
		QTreeWidgetItem * edge_item = new QTreeWidgetItem(ui_.inspector_tree_widget);
		edge_item->setText(0, tr("Edges"));
		edge_item->setText(1, QString("%1").arg(robot_data->get_number_of_messages()));
		edge_item->setExpanded(true);

		std::vector<boost::shared_ptr<EdgeIdentifier> > edges = robot_data->get_edges();
		std::vector<boost::shared_ptr<EdgeIdentifier> >::const_iterator it;
		for( it = edges.begin(); it != edges.end(); ++it ) {
			item = new QTreeWidgetItem(edge_item);
			item->setText(0, tr("Edge"));
			item->setText(1, QString("%1").arg((*it)->id()));
			item->setData(0,Qt::UserRole,QVariant::fromValue(*it));
		}

		// Messages
		QTreeWidgetItem * message_item = new QTreeWidgetItem(ui_.inspector_tree_widget);
		message_item->setText(0, tr("Messages"));
		message_item->setText(1, QString("%1").arg(robot_data->get_number_of_messages()));
		message_item->setExpanded(true);

		for( std::size_t i = 0; i < robot_data->get_number_of_messages(); ++i ) {
			item = new QTreeWidgetItem(message_item);
			item->setText(0, tr("Message"));
			item->setText(1, QString("%1").arg(robot_data->get_message(i)->id()));
			item->setData(0,Qt::UserRole,QVariant::fromValue(robot_data->get_message(i)));
		}
    } else if(e_id.get()) {
    	boost::shared_ptr<Edge> edge = boost::dynamic_pointer_cast<Edge>(data);

		item = new QTreeWidgetItem(ui_.inspector_tree_widget);
		item->setText(0, tr("Source"));
		item->setText(1, QString("%1").arg(edge->getRobot1()->id()));
		item->setData(0,Qt::UserRole,QVariant::fromValue(edge->getRobot1()));

		item = new QTreeWidgetItem(ui_.inspector_tree_widget);
		item->setText(0, tr("Target"));
		item->setText(1, QString("%1").arg(edge->getRobot2()->id()));
		item->setData(0,Qt::UserRole,QVariant::fromValue(edge->getRobot2()));
    }

}

void RSSMainWindow::update_selected_object() {
	if(!selection_id_.get())
		return;

    boost::shared_ptr<WorldInformation> world_info = rss_gl_widget_->simulation_renderer()->world_info();

    boost::shared_ptr<WorldObject> data;
	boost::shared_ptr<RobotIdentifier> r_id = boost::dynamic_pointer_cast<RobotIdentifier>(selection_id_);
	boost::shared_ptr<MessageIdentifier> m_id = boost::dynamic_pointer_cast<MessageIdentifier>(selection_id_);
	boost::shared_ptr<EdgeIdentifier> e_id = boost::dynamic_pointer_cast<EdgeIdentifier>(selection_id_);
	if(r_id.get() && world_info->robot_data().size() > r_id->id() ) {
		data = world_info->get_according_robot_data_ptr(r_id);
	} else if(m_id.get() && world_info->messages().find(m_id->id()) != world_info->messages().end() ) {
		data = world_info->get_according_message(m_id);
	} else if(e_id.get() && world_info->edges().find(e_id->id()) != world_info->edges().end() ) {
		data = world_info->get_according_edge(e_id);
	} else {
		return;
	}

    std::vector<std::string> keys = data->marker_information().get_keys();
    std::vector<std::string>::const_iterator key_it = keys.begin();
	QTreeWidgetItemIterator it(ui_.inspector_tree_widget);
	while(*it) {
		if(key_it != keys.end() && (*it)->text(0) == (*key_it).data()) {
			// Marker information
	    	boost::any m_data = data->marker_information().get_data(*key_it);
			(*it)->setText(1, QString::fromStdString(boost_any_to_string(&m_data)));
			++key_it;
		} else if((*it)->text(0) == tr("Position")) {
			(*it)->setText(1, QString("(%1, %2, %3)")
		    		.arg(data->position()[0])
		    		.arg(data->position()[1])
		    		.arg(data->position()[2]));
		} else if(r_id.get() && (*it)->text(0) == tr("Messages")) {
	    	boost::shared_ptr<RobotData> robot_data = boost::dynamic_pointer_cast<RobotData>(data);
	    	std::size_t m_count = robot_data->get_number_of_messages();
			(*it)->setText(1, QString("%1").arg(m_count));

			while(m_count < (*it)->childCount()) {
				QTreeWidgetItem* child = (*it)->child((*it)->childCount()-1);
				(*it)->removeChild(child);
				delete child;
			}
			for( std::size_t i = 0; i < m_count; ++i ) {
				QTreeWidgetItem * item = i < (*it)->childCount() ? (*it)->child(i) : new QTreeWidgetItem(*it);
				item->setText(0, tr("Message"));
				item->setText(1, QString("%1").arg(robot_data->get_message(i)->id()));
				item->setData(0,Qt::UserRole,QVariant::fromValue(robot_data->get_message(i)));
			}
		} else if(r_id.get() && (*it)->text(0) == tr("Edges")) {
	    	boost::shared_ptr<RobotData> robot_data = boost::dynamic_pointer_cast<RobotData>(data);
			std::vector<boost::shared_ptr<EdgeIdentifier> > edges = robot_data->get_edges();

			(*it)->setText(1, QString("%1").arg(edges.size()));

			while(edges.size() < (*it)->childCount()) {
				QTreeWidgetItem* child = (*it)->child((*it)->childCount()-1);
				(*it)->removeChild(child);
				delete child;
			}
			for( std::size_t i = 0; i < edges.size(); ++i ) {
				QTreeWidgetItem * item = i < (*it)->childCount() ? (*it)->child(i) : new QTreeWidgetItem(*it);
				item->setText(0, tr("Edge"));
				item->setText(1, QString("%1").arg(edges.at(i)->id()));
				item->setData(0,Qt::UserRole,QVariant::fromValue(edges.at(i)));
			}
		} else if(e_id.get() && (*it)->text(0) == tr("Source")) {
	    	boost::shared_ptr<Edge> edge = boost::dynamic_pointer_cast<Edge>(data);
	    	(*it)->setText(1, QString("%1").arg(edge->getRobot1()->id()));
	    	(*it)->setData(0,Qt::UserRole,QVariant::fromValue(edge->getRobot1()));
		} else if(e_id.get() && (*it)->text(0) == tr("Target")) {
	    	boost::shared_ptr<Edge> edge = boost::dynamic_pointer_cast<Edge>(data);
	    	(*it)->setText(1, QString("%1").arg(edge->getRobot2()->id()));
	    	(*it)->setData(0,Qt::UserRole,QVariant::fromValue(edge->getRobot2()));
		}
		++it;
	}

}

void RSSMainWindow::tree_selection_changed(QTreeWidgetItem* item, int column) {
	if(item->text(0) == tr("Message")) {
		boost::shared_ptr<MessageIdentifier> id = item->data(0, Qt::UserRole).value<boost::shared_ptr<MessageIdentifier> >();
		if(id.get())
			selected_object_changed(id);
	} else if(item->text(0) == tr("Edge")) {
		boost::shared_ptr<EdgeIdentifier> id = item->data(0, Qt::UserRole).value<boost::shared_ptr<EdgeIdentifier> >();
		if(id.get())
			selected_object_changed(id);
	} else if(item->text(0) == tr("Robot")) {
		boost::shared_ptr<RobotIdentifier> id = item->data(0, Qt::UserRole).value<boost::shared_ptr<RobotIdentifier> >();
		if(id.get())
			selected_object_changed(id);
	}
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
