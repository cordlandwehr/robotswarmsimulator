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
#include <QSettings>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <Utilities/console_output.h>
#include <SimulationControl/simulation_control.h>
#include <Wrapper/lua_distribution_generator.h>
#include "../Model/world_information.h"
#include "../Model/message_identifier.h"
#include "../Model/message.h"
#include "../Model/robot_identifier.h"
#include "../Model/robot_data.h"

#include "open_project_dialog.h"
#include "generator_wizard.h"
#include "rss_gl_widget.h"
#include "help_dialog.h"


Q_DECLARE_METATYPE(boost::shared_ptr<Identifier>);
Q_DECLARE_METATYPE(boost::shared_ptr<MessageIdentifier>);
Q_DECLARE_METATYPE(boost::shared_ptr<EdgeIdentifier>);
Q_DECLARE_METATYPE(boost::shared_ptr<RobotIdentifier>);


enum { ROBOT_ROLE=1, EDGE_ROLE, MESSAGE_ROLE };

void set_tree_item_data(QTreeWidgetItem* item, const QString& name, const QString& value, bool is_link=false, int link_role=0, const QVariant& link_id = QVariant()) {
	item->setText(0, name);
	item->setText(1, value);
	if(is_link) {
		QFont font = item->font(0);
		font.setUnderline(true);
		item->setFont(0, font);
		item->setIcon(0, QIcon(":icons/small_link.png"));
		item->setData(0,Qt::UserRole,link_id);
		item->setData(0,Qt::UserRole+link_role,true);
	}
}

QAction* create_hidden_action(const QString& name, const QString& object_name, int key, QWidget *parent) {
	QAction* action = new QAction(name, parent);
	action->setObjectName(object_name);
	action->setShortcut(QKeySequence(key));
	parent->addAction(action);
	return action;
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

RSSMainWindow::RSSMainWindow(QWidget *parent) : QMainWindow(parent) {
	ui_.setupUi(this);
	open_dialog_ = new OpenProjectDialog(this);
	generator_wizard_ = new GeneratorWizard(this);
	rss_gl_widget_ = new RSSGLWidget(this);
	help_dialog_ = new HelpDialog(this);
	about_dialog_ = new QDialog(this);

	Ui::AboutDialog about_dialog;
	about_dialog.setupUi(about_dialog_);
}

RSSMainWindow::~RSSMainWindow() {
	delete open_dialog_;
	delete generator_wizard_;
	delete rss_gl_widget_;
	delete help_dialog_;
	delete about_dialog_;
}

void RSSMainWindow::init() {

	this->setCentralWidget(rss_gl_widget_);

	// signal mappers
	QSignalMapper * speed_signal_mapper = new QSignalMapper(this);
	QSignalMapper * cam_signal_mapper = new QSignalMapper(this);
	QSignalMapper * cam_move_signal_mapper = new QSignalMapper(this);
	QSignalMapper * cam_speed_signal_mapper = new QSignalMapper(this);
	QSignalMapper * cam_ortho_axis_mapper = new QSignalMapper(this);

	// hidden actions
	QAction* action_move_forward = create_hidden_action("Move Forward", "action_move_forward", Qt::Key_W, this);
	QAction* action_move_backward = create_hidden_action("Move Backward", "action_move_backward", Qt::Key_S, this);
	QAction* action_move_up = create_hidden_action("Move Up", "action_move_up", Qt::Key_R, this);
	QAction* action_move_down = create_hidden_action("Move Down", "action_move_down", Qt::Key_F, this);
	QAction* action_move_left = create_hidden_action("Move Left", "action_move_left", Qt::Key_A, this);
	QAction* action_move_right = create_hidden_action("Move Right", "action_move_right", Qt::Key_D, this);
	QAction* action_toggle_cam = create_hidden_action("Toggle Camera Mode", "action_toggle_cam", Qt::Key_C, this);
	QAction* action_double_cam_speed = create_hidden_action("Double Camera Speed", "action_double_cam_speed", Qt::Key_PageUp, this);
	QAction* action_half_cam_speed = create_hidden_action("Half Camera Speed", "action_half_cam_speed", Qt::Key_PageDown, this);
	QAction* action_ortho_axis_x = create_hidden_action("Orthogonal Axis X", "action_ortho_axis_x", Qt::Key_X, this);
	QAction* action_ortho_axis_y = create_hidden_action("Orthogonal Axis Y", "action_ortho_axis_y", Qt::Key_Y, this);
	QAction* action_ortho_axis_z = create_hidden_action("Orthogonal Axis Z", "action_ortho_axis_z", Qt::Key_Z, this);

	// action groups
	QActionGroup* action_group = new QActionGroup(this);
	action_group->addAction(ui_.action_free_cam);
	action_group->addAction(ui_.action_follow_swarm_cam);
	action_group->addAction(ui_.action_center_of_gravity_cam);
	action_group->addAction(ui_.action_orthogonal_cam);

	// connect signals/slots
	connect(ui_.action_open_project, SIGNAL(triggered()), open_dialog_, SLOT(show()));
	connect(ui_.action_new_project, SIGNAL(triggered()), generator_wizard_, SLOT(show()));
	connect(ui_.action_about, SIGNAL(triggered()), about_dialog_, SLOT(show()));
	connect(ui_.action_help, SIGNAL(triggered()), help_dialog_, SLOT(show()));
	connect(ui_.action_help, SIGNAL(triggered()), this, SLOT(update_help_window()));
	connect(open_dialog_, SIGNAL(accepted()), this, SLOT(update_simulation()));
	connect(generator_wizard_, SIGNAL(accepted()), this, SLOT(generate_simulation()));
	connect(ui_.action_view_center_of_gravity, SIGNAL(triggered(bool)), rss_gl_widget_, SLOT(toggle_view_cog(bool)));
	connect(ui_.action_view_global_cs, SIGNAL(triggered(bool)), rss_gl_widget_, SLOT(toggle_view_global_cs(bool)));
	connect(ui_.action_view_local_cs, SIGNAL(triggered(bool)), rss_gl_widget_, SLOT(toggle_view_local_cs(bool)));
	connect(ui_.action_start_stop_simulation, SIGNAL(triggered()), this, SLOT(toggle_simulation()));
	connect(ui_.action_next_step, SIGNAL(triggered()), this, SLOT(step_simulation()));
	connect(ui_.action_increase_speed, SIGNAL(triggered()), speed_signal_mapper, SLOT(map()));
	connect(ui_.action_decrease_speed, SIGNAL(triggered()), speed_signal_mapper, SLOT(map()));
	connect(ui_.action_half_speed, SIGNAL(triggered()), speed_signal_mapper, SLOT(map()));
	connect(ui_.action_double_speed, SIGNAL(triggered()), speed_signal_mapper, SLOT(map()));
	connect(ui_.action_free_cam, SIGNAL(triggered()), cam_signal_mapper, SLOT(map()));
	connect(ui_.action_follow_swarm_cam, SIGNAL(triggered()), cam_signal_mapper, SLOT(map()));
	connect(ui_.action_center_of_gravity_cam, SIGNAL(triggered()), cam_signal_mapper, SLOT(map()));
	connect(ui_.action_orthogonal_cam, SIGNAL(triggered()), cam_signal_mapper, SLOT(map()));
	connect(action_move_forward, SIGNAL(triggered()), cam_move_signal_mapper, SLOT(map()));
	connect(action_move_backward, SIGNAL(triggered()), cam_move_signal_mapper, SLOT(map()));
	connect(action_move_left, SIGNAL(triggered()), cam_move_signal_mapper, SLOT(map()));
	connect(action_move_right, SIGNAL(triggered()), cam_move_signal_mapper, SLOT(map()));
	connect(action_move_up, SIGNAL(triggered()), cam_move_signal_mapper, SLOT(map()));
	connect(action_move_down, SIGNAL(triggered()), cam_move_signal_mapper, SLOT(map()));
	connect(action_double_cam_speed, SIGNAL(triggered()), cam_speed_signal_mapper, SLOT(map()));
	connect(action_half_cam_speed, SIGNAL(triggered()), cam_speed_signal_mapper, SLOT(map()));
	connect(action_toggle_cam, SIGNAL(triggered()), rss_gl_widget_, SLOT(toggle_camera_mode()));
	connect(action_ortho_axis_x, SIGNAL(triggered()), cam_ortho_axis_mapper, SLOT(map()));
	connect(action_ortho_axis_y, SIGNAL(triggered()), cam_ortho_axis_mapper, SLOT(map()));
	connect(action_ortho_axis_z, SIGNAL(triggered()), cam_ortho_axis_mapper, SLOT(map()));
	connect(speed_signal_mapper, SIGNAL(mapped(int)), this, SLOT(update_simulation_speed(int)));
	connect(cam_speed_signal_mapper, SIGNAL(mapped(int)), rss_gl_widget_, SLOT(set_camera_speed(int)));
	connect(cam_signal_mapper, SIGNAL(mapped(int)), rss_gl_widget_, SLOT(set_camera_mode(int)));
	connect(cam_move_signal_mapper, SIGNAL(mapped(int)), rss_gl_widget_, SLOT(move_camera(int)));
	connect(cam_ortho_axis_mapper, SIGNAL(mapped(int)), rss_gl_widget_, SLOT(set_orthogonal_axis(int)));
	connect(rss_gl_widget_, SIGNAL(selected_object_changed(boost::shared_ptr<Identifier>)), this, SLOT(selected_object_changed(boost::shared_ptr<Identifier>)));
	connect(ui_.inspector_tree_widget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(tree_selection_changed(QTreeWidgetItem*,int)));

	// map signals
	speed_signal_mapper->setMapping(ui_.action_increase_speed, INCREASE_SPEED);
	speed_signal_mapper->setMapping(ui_.action_decrease_speed, DECREASE_SPEED);
	speed_signal_mapper->setMapping(ui_.action_half_speed, HALF_SPEED);
	speed_signal_mapper->setMapping(ui_.action_double_speed, DOUBLE_SPEED);
	cam_signal_mapper->setMapping(ui_.action_free_cam, RSSGLWidget::CAM_FREE);
	cam_signal_mapper->setMapping(ui_.action_follow_swarm_cam, RSSGLWidget::CAM_FOLLOW);
	cam_signal_mapper->setMapping(ui_.action_center_of_gravity_cam, RSSGLWidget::CAM_COG);
	cam_signal_mapper->setMapping(ui_.action_orthogonal_cam, RSSGLWidget::CAM_ORTHO);
	cam_move_signal_mapper->setMapping(action_move_forward, RSSGLWidget::FORWARD);
	cam_move_signal_mapper->setMapping(action_move_backward, RSSGLWidget::BACKWARD);
	cam_move_signal_mapper->setMapping(action_move_up, RSSGLWidget::UP);
	cam_move_signal_mapper->setMapping(action_move_down, RSSGLWidget::DOWN);
	cam_move_signal_mapper->setMapping(action_move_left, RSSGLWidget::LEFT);
	cam_move_signal_mapper->setMapping(action_move_right, RSSGLWidget::RIGHT);
	cam_speed_signal_mapper->setMapping(action_double_cam_speed, RSSGLWidget::DOUBLE_SPEED);
	cam_speed_signal_mapper->setMapping(action_half_cam_speed, RSSGLWidget::HALF_SPEED);
	cam_ortho_axis_mapper->setMapping(action_ortho_axis_x, RSSGLWidget::ORTHO_AXIS_X);
	cam_ortho_axis_mapper->setMapping(action_ortho_axis_y, RSSGLWidget::ORTHO_AXIS_Y);
	cam_ortho_axis_mapper->setMapping(action_ortho_axis_z, RSSGLWidget::ORTHO_AXIS_Z);

	// setup icons
	QCommonStyle style;
	ui_.action_new_project->setIcon(style.standardIcon(QStyle::SP_FileIcon));
	ui_.action_open_project->setIcon(style.standardIcon(QStyle::SP_DirOpenIcon));
	ui_.action_quit->setIcon(style.standardIcon(QStyle::SP_DialogCloseButton));

	readSettings();

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

void RSSMainWindow::timerEvent(QTimerEvent * event) {
	update_selected_object();
}

void RSSMainWindow::selected_object_changed(boost::shared_ptr<Identifier> id) {

	if(!id.get()) {
		return;
	}
	selection_id_ = id;
	rss_gl_widget_->simulation_renderer()->set_selected_object(id);
	ui_.inspector_tree_widget->clear();

	QString type = tr("Object");
	QVariant id_data;

    boost::shared_ptr<WorldInformation> world_info = rss_gl_widget_->simulation_renderer()->world_info();
    if(!world_info.get())
    	return;

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
	set_tree_item_data(new QTreeWidgetItem(ui_.inspector_tree_widget), type, QString("%1").arg(id->id()));

	// Position
	set_tree_item_data(new QTreeWidgetItem(ui_.inspector_tree_widget), tr("Position"),
			QString("(%1, %2, %3)").arg(data->position()[0]).arg(data->position()[1]).arg(data->position()[2]));

	// Marker information
    QTreeWidgetItem * marker_info_item = new QTreeWidgetItem(ui_.inspector_tree_widget);
    marker_info_item->setText(0, tr("Marker Information"));
	marker_info_item->setExpanded(true);

    std::vector<std::string> keys = data->marker_information().get_keys();
    std::vector<std::string>::const_iterator it;
    for( it = keys.begin(); it != keys.end(); ++it ) {
    	boost::any m_data = data->marker_information().get_data(*it);
    	set_tree_item_data(new QTreeWidgetItem(marker_info_item), QString::fromStdString(*it),
    			QString::fromStdString(boost_any_to_string(&m_data)));
    }

    if(r_id.get()) { // is object a robot?
    	boost::shared_ptr<RobotData> robot_data = boost::dynamic_pointer_cast<RobotData>(data);

		// Edges
		QTreeWidgetItem * edge_item = new QTreeWidgetItem(ui_.inspector_tree_widget);
		set_tree_item_data(edge_item, tr("Edges"), QString("%1").arg(robot_data->get_edges().size()));
		edge_item->setExpanded(true);

		std::vector<boost::shared_ptr<EdgeIdentifier> > edges = robot_data->get_edges();
		std::vector<boost::shared_ptr<EdgeIdentifier> >::const_iterator it;
		for( it = edges.begin(); it != edges.end(); ++it ) {
			set_tree_item_data(new QTreeWidgetItem(edge_item), tr("Edge"), QString("%1").arg((*it)->id()),
					true, EDGE_ROLE,QVariant::fromValue(*it) );
		}

		// Messages
		QTreeWidgetItem * message_item = new QTreeWidgetItem(ui_.inspector_tree_widget);
		set_tree_item_data(message_item, tr("Messages"), QString("%1").arg(robot_data->get_number_of_messages()));
		message_item->setExpanded(true);

		for( std::size_t i = 0; i < robot_data->get_number_of_messages(); ++i ) {
			set_tree_item_data(new QTreeWidgetItem(message_item), tr("Message"), QString("%1").arg(robot_data->get_message(i)->id()),
					true, MESSAGE_ROLE, QVariant::fromValue(robot_data->get_message(i)));
		}
    } else if(e_id.get()) { // is object an edge?
    	boost::shared_ptr<Edge> edge = boost::dynamic_pointer_cast<Edge>(data);

		set_tree_item_data(new QTreeWidgetItem(ui_.inspector_tree_widget), tr("Source"), QString("%1").arg(edge->robot1()->id()),
				true, ROBOT_ROLE, QVariant::fromValue(edge->robot1()));

		set_tree_item_data(new QTreeWidgetItem(ui_.inspector_tree_widget), tr("Target"), QString("%1").arg(edge->robot2()->id()),
				true, ROBOT_ROLE, QVariant::fromValue(edge->robot2()));

		if(boost::dynamic_pointer_cast<DirectedEdge>(data).get()) {
			set_tree_item_data(new QTreeWidgetItem(ui_.inspector_tree_widget), tr("Type"), tr("Directed"));
		} else {
			set_tree_item_data(new QTreeWidgetItem(ui_.inspector_tree_widget), tr("Type"), tr("Undirected"));
		}
    }

}

void RSSMainWindow::update_selected_object() {
	if(!selection_id_.get())
		return;

    boost::shared_ptr<WorldInformation> world_info = rss_gl_widget_->simulation_renderer()->world_info();
    if(!world_info.get())
    	return;

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
		if((*it)->text(0) == tr("Marker Information")) {
			// marker information

		    std::vector<std::string> keys = data->marker_information().get_keys();
			while(keys.size() < (*it)->childCount()) {
				QTreeWidgetItem* child = (*it)->child((*it)->childCount()-1);
				(*it)->removeChild(child);
				delete child;
			}
		    for( std::size_t i=0; i<keys.size(); ++i ) {
		    	boost::any m_data = data->marker_information().get_data(keys.at(i));
				QTreeWidgetItem * item = i < (*it)->childCount() ? (*it)->child(i) : new QTreeWidgetItem(*it);
				set_tree_item_data(item, QString::fromStdString(keys.at(i)), QString::fromStdString(boost_any_to_string(&m_data)));
		    }
		} else if((*it)->text(0) == tr("Position")) {
			// position
			(*it)->setText(1, QString("(%1, %2, %3)").arg(data->position()[0]).arg(data->position()[1]).arg(data->position()[2]));
		} else if(r_id.get() && (*it)->text(0) == tr("Messages")) {
			// messages
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
				set_tree_item_data(item, tr("Message"), QString("%1").arg(robot_data->get_message(i)->id()),
						true, MESSAGE_ROLE, QVariant::fromValue(robot_data->get_message(i)));
			}
		} else if(r_id.get() && (*it)->text(0) == tr("Edges")) {
			// edges
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
				set_tree_item_data(item, tr("Edge"), QString("%1").arg(edges.at(i)->id()),
						true, EDGE_ROLE, QVariant::fromValue(edges.at(i)));
			}
		} else if(e_id.get() && (*it)->text(0) == tr("Source")) {
	    	boost::shared_ptr<Edge> edge = boost::dynamic_pointer_cast<Edge>(data);
			set_tree_item_data(*it, (*it)->text(0), QString("%1").arg(edge->robot1()->id()),
					true, EDGE_ROLE, QVariant::fromValue(edge->robot1()));
		} else if(e_id.get() && (*it)->text(0) == tr("Target")) {
	    	boost::shared_ptr<Edge> edge = boost::dynamic_pointer_cast<Edge>(data);
			set_tree_item_data(*it, (*it)->text(0), QString("%1").arg(edge->robot2()->id()),
					true, EDGE_ROLE, QVariant::fromValue(edge->robot2()));
		}
		++it;
	}

}

void RSSMainWindow::tree_selection_changed(QTreeWidgetItem* item, int column) {
	boost::shared_ptr<Identifier> id;
	 if(item->data(0, Qt::UserRole+ROBOT_ROLE).toBool()) {
		id = item->data(0, Qt::UserRole).value<boost::shared_ptr<RobotIdentifier> >();
	} else if(item->data(0, Qt::UserRole+EDGE_ROLE).toBool()) {
		id = item->data(0, Qt::UserRole).value<boost::shared_ptr<EdgeIdentifier> >();
	} else if(item->data(0, Qt::UserRole+MESSAGE_ROLE).toBool()) {
		id = item->data(0, Qt::UserRole).value<boost::shared_ptr<MessageIdentifier> >();
	}
	if(id.get())
		selected_object_changed(id);
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
	try {
		selection_id_.reset();
		ui_.inspector_tree_widget->clear();

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


		// create simulation kernel
		boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
		if(pd.steps > 0) {
			std::cout <<  "creating simulation with limited steps" << std::endl;
			sim_control->create_new_simulation(tmpProjectFile,
											   pd.history_length,
											   pd.output,
											   true,
											   pd.steps,
											   run_until_no_multiplicity);
		} else {
			std::cout <<  "creating simulation without limited steps" << std::endl;
			sim_control->create_new_simulation(tmpProjectFile,
											   pd.history_length,
											   pd.output,
											   false,
											   0,
											   run_until_no_multiplicity);
		}

		rss_gl_widget_->set_simulation_control(sim_control);
	}
	catch(std::exception& e) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << e.what();
	}
	catch(...) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "Uncaught unknown exception.\n";
	}
}


void RSSMainWindow::closeEvent(QCloseEvent *event) {
	writeSettings();
	event->accept();
}

void RSSMainWindow::writeSettings() {
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "RobotSwarmSimulator", "RobotSwarmSimulator");

	settings.beginGroup("Keys");
	QList<QAction*> actions = this->actions();
	actions.append(ui_.menu_project->actions());
	actions.append(ui_.menu_Simulation->actions());
	actions.append(ui_.menu_Help->actions());
	actions.append(ui_.menu_View->actions());
	actions.append(ui_.menu_camera_mode->actions());

	QList<QAction*>::iterator it;
	for( it = actions.begin(); it != actions.end(); ++it ) {
		if(!(*it)->objectName().isEmpty() ) {
			QVariant shortcut = (*it)->shortcut();
			settings.setValue((*it)->objectName(), shortcut);
		}
	}
	settings.endGroup();
}

void RSSMainWindow::readSettings() {
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "RobotSwarmSimulator", "RobotSwarmSimulator");

	settings.beginGroup("Keys");
	QList<QAction*> actions = this->actions();
	actions.append(ui_.menu_project->actions());
	actions.append(ui_.menu_Simulation->actions());
	actions.append(ui_.menu_Help->actions());
	actions.append(ui_.menu_View->actions());

	QList<QAction*>::iterator it;
	for( it = actions.begin(); it != actions.end(); ++it ) {
		if(!(*it)->objectName().isEmpty() && settings.contains((*it)->objectName())) {
			QKeySequence shortcut = settings.value((*it)->objectName()).value<QKeySequence>();
			(*it)->setShortcut(shortcut);
		}
	}
	settings.endGroup();
}

void RSSMainWindow::update_help_window() {
	readSettings();

	QList<QAction*> actions = this->actions();
	actions.append(ui_.menu_project->actions());
	actions.append(ui_.menu_Simulation->actions());
	actions.append(ui_.menu_Help->actions());
	actions.append(ui_.menu_View->actions());

	help_dialog_->update_table(actions);
}
