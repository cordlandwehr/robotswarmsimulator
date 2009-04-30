/*
 * szenario_generator.cc
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

#include "szenario_generator.h"


void szenario_generator::init(int number_robots, std::string algorithm_id) {
	// the world information object contains all information that shall be saved in the robot file
	generatedWorld_.reset( new WorldInformation());
	parser_.reset(new Parser());

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

void szenario_generator::distribute_robots_uniform(Vector3d boundingBox) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_uniform_real(-boundingBox(kXCoord)/2, boundingBox(kXCoord)/2);
		newRandomPosition->insert_element(kXCoord,png_->get_value_uniform_real());
		png_->init_uniform_real(-boundingBox(kYCoord)/2, boundingBox(kYCoord)/2);
		newRandomPosition->insert_element(kYCoord,png_->get_value_uniform_real());
		png_->init_uniform_real(-boundingBox(kZCoord)/2, boundingBox(kZCoord)/2);
		newRandomPosition->insert_element(kZCoord,png_->get_value_uniform_real());

		(*iter)->set_position( newRandomPosition );
	}
}

void szenario_generator::distribute_robots_normal(Vector3d boundingBox, double mean, double sigma) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_normal(mean, sigma);		// set up the generator
		double randomValue;					// random value to be generated

		/*
		 * generates values according normal distribution until value inside bounding box is found
		 * if one side ob box is equal to zero no limit is asssumed, cause probability to be zero is exactly 0 ;)
		 */
		do {
			randomValue = png_->get_value_normal();
			newRandomPosition->insert_element(kXCoord,randomValue);
		} while (
				randomValue + boundingBox(kXCoord)/2.0 > boundingBox(kXCoord) ||
				randomValue + boundingBox(kXCoord)/2.0 < 0.0 ||
			 	boundingBox(kXCoord)==0.0
			 	);
		do {
			randomValue = png_->get_value_normal();
			newRandomPosition->insert_element(kYCoord,randomValue);
		} while (
				randomValue + boundingBox(kYCoord)/2.0 > boundingBox(kYCoord) ||
				randomValue + boundingBox(kYCoord)/2.0 < 0.0 ||
			 	boundingBox(kYCoord)==0.0
			 	);
		do {
			randomValue = png_->get_value_normal();
			newRandomPosition->insert_element(kZCoord,randomValue);
		} while (
				randomValue + boundingBox(kZCoord)/2.0 > boundingBox(kZCoord) ||
				randomValue + boundingBox(kZCoord)/2.0 < 0.0 ||
			 	boundingBox(kZCoord)==0.0
			 	);

		(*iter)->set_position( newRandomPosition );
	}
}

void szenario_generator::distribute_robots_circle(double radius, double starting_angle) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;
	int circle_num = 0;
	double cur_angle = 0.0;
	double incr_angle = starting_angle;
	const long double pi = 3.1415926535897932384626433832795028841968; // awkward.

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		if(circle_num == 0) {
			// center robot
			boost::shared_ptr<Vector3d> center(new Vector3d());
			center->insert_element(kXCoord,0.0);
			center->insert_element(kYCoord,0.0);
			center->insert_element(kZCoord,0.0);
			(*iter)->set_position( center );

			// this robot is a chieftain
			(*iter)->set_type( MASTER );
			circle_num++;
		} else {
			// position robot on circle i at cur_angle
			boost::shared_ptr<Vector3d> pos(new Vector3d());
			pos->insert_element(kXCoord,std::cos(cur_angle));
			pos->insert_element(kYCoord,std::sin(cur_angle));
			pos->insert_element(kZCoord,0.0);
			(*pos) *= (circle_num * radius);
			(*iter)->set_position( pos );

			// adjust counter variables
			cur_angle += incr_angle;
			if(cur_angle > 2 * pi) {
				circle_num++;
				cur_angle = 0.0;
				incr_angle /= 2;
			}
		}
	}
}


void szenario_generator::distribute_velocity_uniform(double minVelocity, double maxVelocity) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		// get vector uniform on unit sphere
		png_->init_uniform_on_sphere(3);
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d(png_->get_value_uniform_on_sphere_3d()));

		// get vector length uniform in range [minVelocity,maxVelocity]
		png_->init_uniform_real(minVelocity, maxVelocity);
		double vecLength = png_->get_value_uniform_real();
		newRandomPosition->insert_element(kXCoord,vecLength * (*newRandomPosition)(kXCoord));
		newRandomPosition->insert_element(kYCoord,vecLength * (*newRandomPosition)(kYCoord));
		newRandomPosition->insert_element(kZCoord,vecLength * (*newRandomPosition)(kZCoord));

		(*iter)->set_velocity( newRandomPosition );
	}
}


void szenario_generator::distribute_velocity_normal(double mean, double sigma) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_normal(mean, sigma);		// set up the generator

		newRandomPosition->insert_element(kXCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kYCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kZCoord,png_->get_value_normal());

		(*iter)->set_velocity( newRandomPosition );
	}
}


void szenario_generator::distribute_acceleration_uniform(double minAcc, double maxAcc) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		// get vector uniform on unit sphere
		png_->init_uniform_on_sphere(3);
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d(png_->get_value_uniform_on_sphere_3d()));

		// get vector length uniform in range [minAcc,maxAcc]
		png_->init_uniform_real(minAcc, maxAcc);
		double vecLength = png_->get_value_uniform_real();
		newRandomPosition->insert_element(kXCoord,vecLength * (*newRandomPosition)(kXCoord));
		newRandomPosition->insert_element(kYCoord,vecLength * (*newRandomPosition)(kYCoord));
		newRandomPosition->insert_element(kZCoord,vecLength * (*newRandomPosition)(kZCoord));

		(*iter)->set_acceleration( newRandomPosition );
	}
}


void szenario_generator::distribute_acceleration_normal(double mean, double sigma) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_normal(mean, sigma);		// set up the generator

		newRandomPosition->insert_element(kXCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kYCoord,png_->get_value_normal());
		newRandomPosition->insert_element(kZCoord,png_->get_value_normal());

		(*iter)->set_acceleration( newRandomPosition );
	}
}

void szenario_generator::distribute_coordsys_uniform() {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;
	png_->init_uniform_on_sphere(3);	// 3 dimensional

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
					boost::shared_ptr<Vector3d> > newRandomCoordSys;

		do {
			newRandomCoordSys.get<0>().reset(new Vector3d(png_->get_value_uniform_on_sphere_3d()));
			newRandomCoordSys.get<1>().reset(new Vector3d(png_->get_value_uniform_on_sphere_3d()));
			newRandomCoordSys.get<2>().reset(new Vector3d(png_->get_value_uniform_on_sphere_3d()));
		} while (!vector3d_linear_independent(
				*(newRandomCoordSys.get<0>()),
				*(newRandomCoordSys.get<1>()),
				*(newRandomCoordSys.get<2>())
				));

		(*iter)->set_coordinate_system_axis(newRandomCoordSys);
	}
}

void szenario_generator::set_robotFile(std::string filename) {
	robotFileName_.reset(new std::string(filename));
}


void szenario_generator::set_obstacleFile(std::string filename) {
	obstacleFileName_.reset(new std::string(filename));
}


void szenario_generator::set_worldFile(std::string filename) {
	worldFileName_.reset(new std::string(filename));
}


void szenario_generator::set_projectName(std::string projectName) {
	projectName_.reset(new std::string(projectName));
}

void szenario_generator::set_statisticsTemplate(std::string statisticsTemplate) {
	statisticsTemplate_.reset(new std::string(statisticsTemplate));
}


void szenario_generator::set_statisticsSubsets(std::string statisticsSubsets) {
	statisticsSubsets_.reset(new std::string(statisticsSubsets));
}


void szenario_generator::add_play_pos_request_handler() {
	parser_->parameter_map()["POSITION_REQUEST_HANDLER_TYPE"]="VECTOR";
	parser_->parameter_map()["VECTOR_POSITION_REQUEST_HANDLER_SEED"]="1";
	parser_->parameter_map()["VECTOR_POSITION_REQUEST_HANDLER_MODIFIER"]="(VECTOR_DIFFERENCE_TRIMMER,4.0)";
	parser_->parameter_map()["VECTOR_POSITION_REQUEST_HANDLER_DISCARD_PROB"]="0.0";
}


void szenario_generator::add_play_vel_request_handler() {
	parser_->parameter_map()["VELOCITY_REQUEST_HANDLER_TYPE"]="VECTOR";
	parser_->parameter_map()["VECTOR_VELOCITY_REQUEST_HANDLER_SEED"]="1";
	parser_->parameter_map()["VECTOR_VELOCITY_REQUEST_HANDLER_MODIFIER"]="(VECTOR_TRIMMER,0.4)";
	parser_->parameter_map()["VECTOR_VELOCITY_REQUEST_HANDLER_DISCARD_PROB"]="0.0";
}


void szenario_generator::add_play_acc_request_handler() {
	parser_->parameter_map()["ACCELERATION_REQUEST_HANDLER_TYPE"]="VECTOR";
	parser_->parameter_map()["VECTOR_ACCELERATION_REQUEST_HANDLER_SEED"]="1";
	parser_->parameter_map()["VECTOR_ACCELERATION_REQUEST_HANDLER_MODIFIER"]="(VECTOR_TRIMMER,0.2)";
	parser_->parameter_map()["VECTOR_ACCELERATION_REQUEST_HANDLER_DISCARD_PROB"]="0.0";
}


void szenario_generator::write_to_file() {

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


szenario_generator::szenario_generator(int seed) {
	png_.reset(new DistributionGenerator(seed));
}


szenario_generator::~szenario_generator() {

}

