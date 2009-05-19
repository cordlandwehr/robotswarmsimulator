/*
 * ScenarioGenerator.cc
 *
 *  Created on: Jan 19, 2009
 *      Author: phoenixx
 */

#include <boost/unordered_map.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>
#include <string>
#include <cmath>

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "../SimulationKernel/factories.h"

#include "../Utilities/parser.h"
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"

#include "formation_generator.h"
#include "uniform_formation_generator.h"
#include "gaussian_formation_generator.h"
#include "circle_formation_generator.h"
#include "random_walk_formation_generator.h"
#include "cluster_formation_generator.h"
#include "poisson_disc_formation_generator.h"

#include "szenario_generator.h"


void ScenarioGenerator::init(const boost::program_options::variables_map& vm) {

	// initialize the formation generator
	init_formation_generator(vm);

	// the world information object contains all information that shall be saved in the robot file
	generatedWorld_.reset( new WorldInformation());
	parser_.reset(new Parser());

	int number_robots = vm["robots"].as<unsigned int>();
	std::string algorithm_id = vm["algorithm"].as<std::string>();
	for (int ctr=0; ctr < number_robots; ctr++) {
		boost::shared_ptr< RobotIdentifier > tmpIdent;
		boost::shared_ptr< RobotData > tmpRobotData;
		boost::shared_ptr< Robot > tmpRobot;

		tmpIdent.reset(new RobotIdentifier(ctr));

		tmpRobot = Factory::robot_factory(tmpIdent, algorithm_id);

		// create position for new robot: (0,0,0)
		boost::shared_ptr< Vector3d > tmpPos;
		tmpPos.reset(new Vector3d);
		tmpPos->insert_element(kXCoord,0.0);
		tmpPos->insert_element(kYCoord,0.0);
		tmpPos->insert_element(kZCoord,0.0);

		tmpRobotData.reset(new RobotData(tmpIdent, tmpPos, *tmpRobot));

		// create acceleration: (0,0,0)
		boost::shared_ptr<Vector3d> tmpAcc(new Vector3d());
		tmpAcc->insert_element(kXCoord,0.0);
		tmpAcc->insert_element(kYCoord,0.0);
		tmpAcc->insert_element(kZCoord,0.0);
		tmpRobotData->set_acceleration(tmpAcc);

		// create velocity: (0,0,0)
		boost::shared_ptr<Vector3d> tmpVel(new Vector3d());
		tmpVel->insert_element(kXCoord,0.0);
		tmpVel->insert_element(kYCoord,0.0);
		tmpVel->insert_element(kZCoord,0.0);
		tmpRobotData->set_velocity(tmpVel);

		// set stati for robots
		RobotStatus tmpRobotState = READY;
		tmpRobotData->set_status(tmpRobotState);

		//set type of robot
		tmpRobotData->set_type(SLAVE);

		boost::shared_ptr<Vector3d> axis_x(new Vector3d());
		boost::shared_ptr<Vector3d> axis_y(new Vector3d());
		boost::shared_ptr<Vector3d> axis_z(new Vector3d());
		boost::tuple< boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d>,
					  boost::shared_ptr<Vector3d> > axis (axis_x, axis_y, axis_z);
		boost::get<0>(axis)->insert_element(kXCoord, 1.0);
		boost::get<0>(axis)->insert_element(kYCoord, 0.0);
		boost::get<0>(axis)->insert_element(kZCoord, 0.0);
		boost::get<1>(axis)->insert_element(kXCoord, 0.0);
		boost::get<1>(axis)->insert_element(kYCoord, 1.0);
		boost::get<1>(axis)->insert_element(kZCoord, 0.0);
		boost::get<2>(axis)->insert_element(kXCoord, 0.0);
		boost::get<2>(axis)->insert_element(kYCoord, 0.0);
		boost::get<2>(axis)->insert_element(kZCoord, 1.0);
		tmpRobotData->set_coordinate_system_axis(axis);

		robotList_.push_back(tmpRobot);
		robotDataList_.push_back(tmpRobotData);
	}

	// set some default values
	projectName_.reset(new std::string("New generated Project"));
	robotFileName_.reset(new std::string("new_random_robotfile"));
	obstacleFileName_.reset(new std::string("new_random_obstaclefile"));
	worldFileName_.reset(new std::string("new_random_swarmfile"));
	asg_.reset(new std::string("SYNCHRONOUS"));
	compassModel_.reset(new std::string("NO_COMPASS"));
	statisticsTemplate_.reset(new std::string("ALL"));
	statisticsSubsets_.reset(new std::string("{ALL}"));
}

void ScenarioGenerator::distribute() {
	formation_generator_->distribute(robotDataList_);
}

void ScenarioGenerator::init_formation_generator(const boost::program_options::variables_map& vm) {
	// create formation generator
	if (vm["distr-pos"].as<double>() != 0.0) {
		formation_generator_.reset(new UniformFormationGenerator());
	} else if(vm.count("distr-pos-circle")) {
		formation_generator_.reset(new CircleFormationGenerator());
	} else if(vm.count("distr-gauss-pos")) {
		formation_generator_.reset(new GaussianFormationGenerator());
	} else if(vm.count("random-walk")) {
		formation_generator_.reset(new RandomWalkFormationGenerator());
	} else if(vm.count("cluster")) {
		formation_generator_.reset(new ClusterFormationGenerator());
	} else if(vm.count("poisson-disc")) {
		formation_generator_.reset(new PoissonDiscFormationGenerator());
	}
	else // use default formation generator
		formation_generator_.reset(new UniformFormationGenerator());

	// initalize formation generator
	formation_generator_->init(vm);
}

void ScenarioGenerator::distribute_coordsys(const boost::program_options::variables_map& vm) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> axis_x(new Vector3d());
		boost::shared_ptr<Vector3d> axis_y(new Vector3d());
		boost::shared_ptr<Vector3d> axis_z(new Vector3d());
		boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
					boost::shared_ptr<Vector3d> > newRandomCoordSys(axis_x, axis_y, axis_z);

		// set default coordinate system (the canonic one)
		boost::get<0>(newRandomCoordSys)->insert_element(kXCoord, 1.0);
		boost::get<0>(newRandomCoordSys)->insert_element(kYCoord, 0.0);
		boost::get<0>(newRandomCoordSys)->insert_element(kZCoord, 0.0);
		boost::get<1>(newRandomCoordSys)->insert_element(kXCoord, 0.0);
		boost::get<1>(newRandomCoordSys)->insert_element(kYCoord, 1.0);
		boost::get<1>(newRandomCoordSys)->insert_element(kZCoord, 0.0);
		boost::get<2>(newRandomCoordSys)->insert_element(kXCoord, 0.0);
		boost::get<2>(newRandomCoordSys)->insert_element(kYCoord, 0.0);
		boost::get<2>(newRandomCoordSys)->insert_element(kZCoord, 1.0);
		do {
			// generate coordsystem rotations
			png_->init_uniform_on_sphere(3);	// 3 dimensional
			if (vm.count("cosys-rotate-x"))
				newRandomCoordSys.get<0>().reset(new Vector3d(png_->get_value_uniform_on_sphere_3d()));
			if (vm.count("cosys-rotate-y"))
				newRandomCoordSys.get<1>().reset(new Vector3d(png_->get_value_uniform_on_sphere_3d()));
			if (vm.count("cosys-rotate-z"))
				newRandomCoordSys.get<2>().reset(new Vector3d(png_->get_value_uniform_on_sphere_3d()));

			// scale the axis
			png_->init_uniform_real(0.000000001,1);
			if (vm.count("cosys-scale-x")) {
				double scale = png_->get_value_uniform_real();
				boost::get<0>(newRandomCoordSys)->insert_element(kXCoord, (*newRandomCoordSys.get<0>())(kXCoord) * scale);
				boost::get<0>(newRandomCoordSys)->insert_element(kYCoord, (*newRandomCoordSys.get<0>())(kYCoord) * scale);
				boost::get<0>(newRandomCoordSys)->insert_element(kZCoord, (*newRandomCoordSys.get<0>())(kZCoord) * scale);
			}
			if (vm.count("cosys-scale-y")) {
				double scale = png_->get_value_uniform_real();
				boost::get<1>(newRandomCoordSys)->insert_element(kXCoord, (*newRandomCoordSys.get<1>())(kXCoord) * scale);
				boost::get<1>(newRandomCoordSys)->insert_element(kYCoord, (*newRandomCoordSys.get<1>())(kYCoord) * scale);
				boost::get<1>(newRandomCoordSys)->insert_element(kZCoord, (*newRandomCoordSys.get<1>())(kZCoord) * scale);
			}
			if (vm.count("cosys-scale-z")) {
				double scale = png_->get_value_uniform_real();
				boost::get<2>(newRandomCoordSys)->insert_element(kXCoord, (*newRandomCoordSys.get<2>())(kXCoord) * scale);
				boost::get<2>(newRandomCoordSys)->insert_element(kYCoord, (*newRandomCoordSys.get<2>())(kYCoord) * scale);
				boost::get<2>(newRandomCoordSys)->insert_element(kZCoord, (*newRandomCoordSys.get<2>())(kZCoord) * scale);
			}
		} while (!vector3d_linear_independent(
				*(newRandomCoordSys.get<0>()),
				*(newRandomCoordSys.get<1>()),
				*(newRandomCoordSys.get<2>())
				));

		(*iter)->set_coordinate_system_axis(newRandomCoordSys);
	}
}

void ScenarioGenerator::set_robotFile(std::string filename) {
	robotFileName_.reset(new std::string(filename));
}


void ScenarioGenerator::set_obstacleFile(std::string filename) {
	obstacleFileName_.reset(new std::string(filename));
}


void ScenarioGenerator::set_worldFile(std::string filename) {
	worldFileName_.reset(new std::string(filename));
}


void ScenarioGenerator::set_projectName(std::string projectName) {
	projectName_.reset(new std::string(projectName));
}

void ScenarioGenerator::set_statisticsTemplate(std::string statisticsTemplate) {
	statisticsTemplate_.reset(new std::string(statisticsTemplate));
}


void ScenarioGenerator::set_statisticsSubsets(std::string statisticsSubsets) {
	statisticsSubsets_.reset(new std::string(statisticsSubsets));
}


void ScenarioGenerator::add_play_pos_request_handler() {
	parser_->parameter_map()["POSITION_REQUEST_HANDLER_TYPE"]="VECTOR";
	parser_->parameter_map()["VECTOR_POSITION_REQUEST_HANDLER_SEED"]="1";
	parser_->parameter_map()["VECTOR_POSITION_REQUEST_HANDLER_MODIFIER"]="(VECTOR_DIFFERENCE_TRIMMER,4.0)";
	parser_->parameter_map()["VECTOR_POSITION_REQUEST_HANDLER_DISCARD_PROB"]="0.0";
}


void ScenarioGenerator::add_play_vel_request_handler() {
	parser_->parameter_map()["VELOCITY_REQUEST_HANDLER_TYPE"]="VECTOR";
	parser_->parameter_map()["VECTOR_VELOCITY_REQUEST_HANDLER_SEED"]="1";
	parser_->parameter_map()["VECTOR_VELOCITY_REQUEST_HANDLER_MODIFIER"]="(VECTOR_TRIMMER,0.4)";
	parser_->parameter_map()["VECTOR_VELOCITY_REQUEST_HANDLER_DISCARD_PROB"]="0.0";
}


void ScenarioGenerator::add_play_acc_request_handler() {
	parser_->parameter_map()["ACCELERATION_REQUEST_HANDLER_TYPE"]="VECTOR";
	parser_->parameter_map()["VECTOR_ACCELERATION_REQUEST_HANDLER_SEED"]="1";
	parser_->parameter_map()["VECTOR_ACCELERATION_REQUEST_HANDLER_MODIFIER"]="(VECTOR_TRIMMER,0.2)";
	parser_->parameter_map()["VECTOR_ACCELERATION_REQUEST_HANDLER_DISCARD_PROB"]="0.0";
}


void ScenarioGenerator::write_to_file() {

	// setup parser for writing with previously set values.
	parser_->set_obstacle_filename(*obstacleFileName_);
	parser_->set_robot_filename(*robotFileName_);

	parser_->parameter_map()["PROJECT_NAME"] = *projectName_;
	parser_->parameter_map()["ASG"] = *asg_;
	parser_->parameter_map()["COMPASS_MODEL"] = *compassModel_;
	parser_->parameter_map()["STATISTICS_TEMPLATE"] = *statisticsTemplate_;
	parser_->parameter_map()["STATISTICS_SUBSETS"] = *statisticsSubsets_;

	// add additional volatile parameters to parameter_map
	parser_->parameter_map()["STATISTICS_DATADUMP"] = "NONE";

	// add all robots to world information
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;
	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ )
		generatedWorld_->add_robot_data((*iter));

	parser_->save_projectfiles(*worldFileName_, *generatedWorld_);
}


ScenarioGenerator::ScenarioGenerator(int seed) {
	png_.reset(new DistributionGenerator(seed));
}


ScenarioGenerator::~ScenarioGenerator() {

}

