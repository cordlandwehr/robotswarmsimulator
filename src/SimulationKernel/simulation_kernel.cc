#include "simulation_kernel.h"

#include "../EventHandlers/event_handler.h"
#include "../EventHandlers/marker_request_handler.h"
#include "../EventHandlers/type_change_request_handler.h"
#include "../EventHandlers/vector_request_handler.h"

#include "../ActivationSequenceGenerators/activation_sequence_generator.h"
#include "../ActivationSequenceGenerators/synchronous_asg.h"

#include "robot_control.h"
#include "../Views/abstract_view_factory.h"
#include "../Views/view_factory.h"
#include "../Views/global_view.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"

#include "../RobotImplementations/simple_robot.h"

#include <iostream>

/*namespace {

	boost::shared_ptr<WorldInformation> temporary_initial_world_information(vector<boost::shared_ptr<Robot> > &robots) {

	}

}*/

SimulationKernel::SimulationKernel() {
	// initialize ASG and Viewmap
	  ASG_map_["SYNCHRONOUS"] = SYNCHRONOUS;
	  ASG_map_["SEMISYNCHRONOUS"] = SEMISYNCHRONOUS;
	  ASG_map_["ASYNCHRONOUS"] = ASYNCHRONOUS;

	  view_map_["FULLVIEW"] = FULLVIEW;
	  view_map_["GLOBALVIEW"] = GLOBALVIEW;
}

SimulationKernel::~SimulationKernel() {
	// TODO Auto-generated destructor stub
}

const vector<boost::shared_ptr<Robot> >& SimulationKernel::robots() const {
	return robots_;
}

const boost::shared_ptr<History>& SimulationKernel::history() const {
	return history_;
}

void SimulationKernel::init(const string& project_filename, boost::shared_ptr<History> history) {

	// set history
	history_ = history;

	// create Parser and load project file
	boost::shared_ptr<Parser> parser;
	parser.reset(new Parser());

	//Workaround: this line would be correct
//	parser->load_projectfiles(project_filename);
	//We use this line for testpurposes to have some running configuration
	parser->load_projectfiles("src/Tests/TestData/testfile_1");

	// create and add initial world information to history
	boost::shared_ptr<WorldInformation> initial_world_information = setup_initial_world_information(parser);
	history_->insert(initial_world_information);

	// create View
	boost::shared_ptr<AbstractViewFactory> view_factory;

	switch(view_map_[parser->view()]) {

		case GLOBALVIEW:
			view_factory.reset(new ViewFactory<GlobalView>());
			break;
		case FULLVIEW:
			view_factory.reset(new ViewFactory<FullView>());
			break;
	}

	// create Robot Control
	boost::shared_ptr<RobotControl> robot_control(new RobotControl(view_factory, history_->size(), *initial_world_information));

	// setup of activation sequence generator
	switch (ASG_map_[parser->asg()]) {

		case SYNCHRONOUS:
			asg_.reset(new SynchronousASG());
			break;
		case SEMISYNCHRONOUS:
			//TODO(mmarcus) make something semi-synchrounous here;
			break;
		case ASYNCHRONOUS:
			//TODO(mmarcus) make something asynchrounous here;
			break;
	}

	asg_->initialize(*history_, robots_);

	// create according EventHandler.
	event_handler_.reset(new EventHandler(history, robot_control));

	// setup of request handlers (all requests handled exactly as requested)
	//TODO(mmarcus) this *history seems a littlebit ugly
	boost::shared_ptr<VectorRequestHandler> request_handler_acc(new VectorRequestHandler(5, 0.0, *history));
	event_handler_->set_acceleration_request_handler(request_handler_acc);

	boost::shared_ptr<VectorRequestHandler> request_handler_vel(new VectorRequestHandler(5, 0.0, *history));
	event_handler_->set_velocity_request_handler(request_handler_vel);

	boost::shared_ptr<VectorRequestHandler> request_handler_pos(new VectorRequestHandler(5, 0.0, *history));
	event_handler_->set_position_request_handler(request_handler_pos);

	boost::shared_ptr<MarkerRequestHandler> request_handler_marker(new MarkerRequestHandler(5, 0.0, *history));
	event_handler_->set_marker_request_handler(request_handler_marker);

	boost::shared_ptr<TypeChangeRequestHandler> request_handler_type(new TypeChangeRequestHandler(5, 0.0, *history));
	event_handler_->set_type_change_request_handler(request_handler_type);

	// create and initialize statistics module;
	stats_.reset(new StatsControl());
	stats_->init(parser);

	// register SimulationObservers (ViewObject, ASG, maybe StatisticObject)
	//TODO(Sven) "stats_->update(initialWorldInformation)"
	event_handler_->register_listener(asg_);
	event_handler_->register_listener(stats_);

}

void SimulationKernel::step() {
	// TODO(craupach): this is a suggestion by me.
	// handle next event
//	boost::shared_ptr<Event> event = asg_->get_next_event();
	//std::cout << "Stepping for Event at time " << event->time() << std::endl;
	event_handler_->handle_event(asg_->get_next_event());
}

void SimulationKernel::multistep(int steps) {
	for (int i = 0; i < steps; i++) {
		step();
	}
}

void SimulationKernel::quit() {
	stats_->quit();
}

//big-TODO(mmarcus) write this method! Everything inside here is just the temporary code of Christoph
boost::shared_ptr<WorldInformation> SimulationKernel::setup_initial_world_information(
		boost::shared_ptr<Parser> parser) {

	// EVERYTHING HERE IS JUST DUMMY-DATA!!! NO DATA IS TAKEN FROM PARSER!!!

	boost::shared_ptr<WorldInformation> initial_world_information;

	// Robot Datas
	boost::shared_ptr<RobotData> robot_data_a;
	boost::shared_ptr<RobotData> robot_data_b;
	boost::shared_ptr<RobotData> robot_data_c;

	// Robots
	boost::shared_ptr<Robot> robot_a;
	boost::shared_ptr<Robot> robot_b;
	boost::shared_ptr<Robot> robot_c;

	// Robot Identifiers
	boost::shared_ptr<RobotIdentifier> id_a;
	boost::shared_ptr<RobotIdentifier> id_b;
	boost::shared_ptr<RobotIdentifier> id_c;

	initial_world_information.reset(new WorldInformation());

	id_a.reset(new RobotIdentifier(0));
	id_b.reset(new RobotIdentifier(1));
	id_c.reset(new RobotIdentifier(2));

	robot_a.reset(new SimpleRobot(id_a));
	robot_b.reset(new SimpleRobot(id_b));
	robot_c.reset(new SimpleRobot(id_c));

	robots_.push_back(robot_a);
	robots_.push_back(robot_b);
	robots_.push_back(robot_c);

	// create position for robot a: (0,0,0)
	boost::shared_ptr<Vector3d> pos_a;
	pos_a.reset(new Vector3d);
	pos_a->insert_element(kXCoord,0.0);
	pos_a->insert_element(kYCoord,0.0);
	pos_a->insert_element(kZCoord,0.0);
	robot_data_a.reset(new RobotData(id_a, pos_a, *robot_a));

	// create position for robot b: (1,0.5,3)
	Vector3d * pos_b_ptr = new Vector3d;
	boost::shared_ptr<Vector3d> pos_b;
	pos_b.reset(pos_b_ptr);
	pos_b->insert_element(kXCoord,1.0);
	pos_b->insert_element(kYCoord,0.5);
	pos_b->insert_element(kZCoord,3.0);
	robot_data_b.reset(new RobotData(id_b, pos_b, *robot_b));

	// create position for robot c: (1.0, 1.0, 1.0)
	boost::shared_ptr<Vector3d> pos_c;
	pos_c.reset(new Vector3d());
	pos_c->insert_element(kXCoord,1.0);
	pos_c->insert_element(kYCoord,1.0);
	pos_c->insert_element(kZCoord,1.0);
	robot_data_c.reset(new RobotData(id_c, pos_c, *robot_c));

	// create velocity for robot a: (0,0,0)
	boost::shared_ptr<Vector3d> vel_a(new Vector3d());
	vel_a->insert_element(kXCoord,0.1);
	vel_a->insert_element(kYCoord,0.1);
	vel_a->insert_element(kZCoord,0.0);
	robot_data_a->set_velocity(vel_a);

	// create acceleration for robot a: (0,0,0)
	boost::shared_ptr<Vector3d> acc_a(new Vector3d());
	acc_a->insert_element(kXCoord,0.0);
	acc_a->insert_element(kYCoord,0.0);
	acc_a->insert_element(kZCoord,0.0);
	robot_data_a->set_acceleration(acc_a);

	// create  velocity for robot b: (0,0,0)
	boost::shared_ptr<Vector3d> vel_b(new Vector3d());
	vel_b->insert_element(kXCoord,0.0);
	vel_b->insert_element(kYCoord,0.0);
	vel_b->insert_element(kZCoord,0.0);
	robot_data_b->set_velocity(vel_b);

	// create acceleration for robot b: (0,0,0)
	boost::shared_ptr<Vector3d> acc_b(new Vector3d());
	acc_b->insert_element(kXCoord,0.0);
	acc_b->insert_element(kYCoord,0.0);
	acc_b->insert_element(kZCoord,0.0);
	robot_data_b->set_acceleration(acc_b);

	// create velocity for robot c: (0,0,0)
	boost::shared_ptr<Vector3d> vel_c(new Vector3d());
	vel_c->insert_element(kXCoord,0.0);
	vel_c->insert_element(kYCoord,0.0);
	vel_c->insert_element(kZCoord,0.0);
	robot_data_c->set_velocity(vel_c);

	// create acceleration for robot c: (0.0, 0.0, 0.0)
	boost::shared_ptr<Vector3d> acc_c(new Vector3d());
	acc_c->insert_element(kXCoord,0.0);
	acc_c->insert_element(kYCoord,0.0);
	acc_c->insert_element(kZCoord,0.0);
	robot_data_c->set_acceleration(acc_c);

	// build a coordinate axes for a robot with unit distance 2 for robot a
	boost::shared_ptr<Vector3d> x_axis_a(new Vector3d());
	x_axis_a->insert_element(kXCoord, 2.0);
	x_axis_a->insert_element(kYCoord, 0.0);
	x_axis_a->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> y_axis_a(new Vector3d());
	y_axis_a->insert_element(kXCoord, 0.0);
	y_axis_a->insert_element(kYCoord, 2.0);
	y_axis_a->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> z_axis_a(new Vector3d());
	z_axis_a->insert_element(kXCoord, 0.0);
	z_axis_a->insert_element(kYCoord, 0.0);
	z_axis_a->insert_element(kZCoord, 2.0);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	boost::shared_ptr<Vector3d> > axes_a(x_axis_a, y_axis_a, z_axis_a);

	robot_data_a->set_coordinate_system_axis(axes_a);

	// build a coordinate axes for a robot with unit distance 1 for robot b
	boost::shared_ptr<Vector3d> x_axis_b(new Vector3d());
	x_axis_b->insert_element(kXCoord, 1.0);
	x_axis_b->insert_element(kYCoord, 0.0);
	x_axis_b->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> y_axis_b(new Vector3d());
	y_axis_b->insert_element(kXCoord, 0.0);
	y_axis_b->insert_element(kYCoord, 1.0);
	y_axis_b->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> z_axis_b(new Vector3d());
	z_axis_b->insert_element(kXCoord, 0.0);
	z_axis_b->insert_element(kYCoord, 0.0);
	z_axis_b->insert_element(kZCoord, 1.0);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	boost::shared_ptr<Vector3d> > axes_b(x_axis_b, y_axis_b, z_axis_b);

	robot_data_b->set_coordinate_system_axis(axes_b);

	// build a coordinate axes with wacky axes for robot c
	boost::shared_ptr<Vector3d> x_axis_c(new Vector3d());
	x_axis_c->insert_element(kXCoord, 1.5);
	x_axis_c->insert_element(kYCoord, 1.0);
	x_axis_c->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> y_axis_c(new Vector3d());
	y_axis_c->insert_element(kXCoord, 0.0);
	y_axis_c->insert_element(kYCoord, 1.9);
	y_axis_c->insert_element(kZCoord, 1.0);

	boost::shared_ptr<Vector3d> z_axis_c(new Vector3d());
	z_axis_c->insert_element(kXCoord, 1.8);
	z_axis_c->insert_element(kYCoord, 0.0);
	z_axis_c->insert_element(kZCoord, 1.3);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	boost::shared_ptr<Vector3d> > axes_c(x_axis_c, y_axis_c, z_axis_c);

	robot_data_c->set_coordinate_system_axis(axes_c);

	// add all robots to the world information
	initial_world_information->add_robot_data(robot_data_a);
	initial_world_information->add_robot_data(robot_data_b);
	initial_world_information->add_robot_data(robot_data_c);

	// set time of inital world information
	initial_world_information->set_time(0);
	return initial_world_information;
}
