/*
 * szenario_generator.cc
 *
 *  Created on: Jan 19, 2009
 *      Author: phoenixx
 */

/*
 * TODO (cola) parser->event_handler_ = 42; not yet implemented for generator
 * TODO (cola) set robot algorithms
 * TODO (cola) coord system
 */

#include <boost/unordered_map.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

//TODO delete
#include <iostream>

#include "../Model/robot_data.h"
#include "../Model/robot.h"
#include "../Model/robot_identifier.h"
#include "../Model/world_information.h"

#include "parser.h"
#include "vector_arithmetics.h"
#include "szenario_generator.h"
#include "distribution_generator.h"


//TODO is there a better way to create robot-datas?
class SimpleRobot : public Robot {
public:
	SimpleRobot(boost::shared_ptr<RobotIdentifier> id, boost::shared_ptr<std::string> algorithm_id)
		: Robot(id, algorithm_id) {}
	std::set<boost::shared_ptr<Request> > compute() {
		std::set<boost::shared_ptr<Request> > empty_set;
		return empty_set;
	}
};


void szenario_generator::init(int number_robots, std::string algorithm_id) {
	// the world information object contains all information that shall be saved in the robot file
	generatedWorld_.reset( new WorldInformation());
	parser_.reset(new Parser());

	for (int ctr=0; ctr < number_robots; ctr++) {
		boost::shared_ptr< RobotIdentifier > tmpIdent;
		boost::shared_ptr< RobotData > tmpRobotData;
		boost::shared_ptr< Robot > tmpRobot;
		boost::shared_ptr< std::string > tmpAlgorithm_id;

		tmpIdent.reset(new RobotIdentifier(ctr));
		tmpAlgorithm_id.reset(new std::string(algorithm_id));

		tmpRobot.reset (new SimpleRobot(tmpIdent, tmpAlgorithm_id));

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

		robotDataList_.push_back(tmpRobotData);
	}

	// set some dummy values
	projectName_.reset(new std::string("New generated Project"));
	robotFileName_.reset(new std::string("new_random_robotfile"));
	obstacleFileName_.reset(new std::string("new_random_obstaclefile"));
	worldFileName_.reset(new std::string("new_random_worldfile"));
	asg_.reset(new std::string("SYNCHRONOUS"));
	compassModel_.reset(new std::string("NO_COMPASS"));
	statisticsTemplate_.reset(new std::string("ALL"));
	statisticsSubsets_.reset(new std::string("{ALL}"));

}

void szenario_generator::distribute_robots_uniform(Vector3d boundingBox) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_uniform(0.0, boundingBox(kXCoord));
		newRandomPosition->insert_element(kXCoord,png_->get_value_uniform());
		png_->init_uniform(0.0, boundingBox(kYCoord));
		newRandomPosition->insert_element(kYCoord,png_->get_value_uniform());
		png_->init_uniform(0.0, boundingBox(kZCoord));
		newRandomPosition->insert_element(kZCoord,png_->get_value_uniform());

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


void szenario_generator::distribute_velocity_uniform(Vector3d boundingBox) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_uniform(0.0, boundingBox(kXCoord));
		newRandomPosition->insert_element(kXCoord,png_->get_value_uniform());
		png_->init_uniform(0.0, boundingBox(kYCoord));
		newRandomPosition->insert_element(kYCoord,png_->get_value_uniform());
		png_->init_uniform(0.0, boundingBox(kZCoord));
		newRandomPosition->insert_element(kZCoord,png_->get_value_uniform());

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


void szenario_generator::distribute_acceleration_uniform(Vector3d boundingBox) {
	std::vector< boost::shared_ptr<RobotData> >::iterator iter;

	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
		boost::shared_ptr<Vector3d> newRandomPosition(new Vector3d());
		png_->init_uniform(0.0, boundingBox(kXCoord));
		newRandomPosition->insert_element(kXCoord,png_->get_value_uniform());
		png_->init_uniform(0.0, boundingBox(kYCoord));
		newRandomPosition->insert_element(kYCoord,png_->get_value_uniform());
		png_->init_uniform(0.0, boundingBox(kZCoord));
		newRandomPosition->insert_element(kZCoord,png_->get_value_uniform());

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

//TODO delete!
//void szenario_generator::set_defaultAlgorithm(std::string algorithm_id) {
//	default_algorithm_.reset(new std::string(algorithm_id));
//	std::vector< boost::shared_ptr<RobotData> >::const_iterator iter;
//	for(iter = robotDataList_.begin(); iter != robotDataList_.end() ; iter++ ) {
//		((*iter)->robot()).set_algorithm_id("a");
//	}
//}

void szenario_generator::set_robotFile(std::string filename) {
	robotFileName_.reset(new std::string(filename));
}


void szenario_generator::set_obstacleFile(string filename) {
	obstacleFileName_.reset(new std::string(filename));
}


void szenario_generator::set_worldFile(string filename) {
	worldFileName_.reset(new std::string(filename));
}


void szenario_generator::set_projectName(string projectName) {
	projectName_.reset(new std::string(projectName));
}


void szenario_generator::set_asg(string asg) {
	asg_.reset(new std::string(asg));
}


void szenario_generator::set_compassModel(string compassModel) {
	compassModel_.reset(new std::string(compassModel));
}


void szenario_generator::set_statisticsTemplate(string statisticsTemplate) {
	statisticsTemplate_.reset(new std::string(statisticsTemplate));
}


void szenario_generator::set_statisticsSubsets(string statisticsSubsets) {
	statisticsSubsets_.reset(new std::string(statisticsSubsets));
}


void szenario_generator::write_to_file() {

	// setup parser for writing with previously set values
	parser_->set_obstacle_filename(*obstacleFileName_);
	parser_->set_project_name(*projectName_);
	parser_->set_robot_filename(*robotFileName_);
	parser_->set_asg(*asg_);
	parser_->set_compass_model(*compassModel_);
	parser_->set_statistics_template(*statisticsTemplate_);
	parser_->set_statistics_subsets(*statisticsSubsets_);

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

