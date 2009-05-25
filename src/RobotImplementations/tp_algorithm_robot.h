/*
 * tp_algorithm_robot.h
 *
 *  Created on: Apr 10, 2009
 *      Author: craupach
 */

#ifndef TP_ALGORITHM_ROBOT_H_
#define TP_ALGORITHM_ROBOT_H_

#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>


#include "../Model/robot.h"
#include "../Utilities/vector_arithmetics.h"
#include "../Utilities/distribution_generator.h"

class RobotIdentifier;
class Request;

#include <iostream>

/**
 * This class is a C implementation of the target point algorithm for all different target points.
 * This means it will execute the following algorithm:
 *
 * 1. Compute specified target point
 * 2. Go there.
 *
 * As new target points get added in the computational geometry package this should be extended.
 * There should not be any more robot implementations for target point algorithms.
 */
class TPAlgorithmRobot : public Robot {
public:
	enum Type {
		cog,
		cminball,
		cbox,
		maxline,
		midfar,
		median,
		rminrect
	};

	TPAlgorithmRobot(boost::shared_ptr<RobotIdentifier> id, Type type) : Robot(id), type_(type) {
		if (type_==rminrect){
			source_of_randomness=boost::shared_ptr<DistributionGenerator>(new DistributionGenerator(1));
		}


	}

	/**
	 * computes a position request for a target point based on the type of the robot
	 */
	std::set<boost::shared_ptr<Request> > compute();

	virtual std::string get_algorithm_id () const {
		switch(type_) {
		case cog:
			return "TPAlgorithmCOG";
		case cminball:
			return "TPAlgorithmCMinball";
		case cbox:
			return "TPAlgorithmCBox";
		case maxline:
			return "TPAlgorithmMaxline";
		case midfar:
			return "TPAlgorithmMidfar";
		case median:
			return "TPAlgorithmMedian";
		case rminrect:
			return "TPAlgorithmRMinRect";
		default:
			return "TPAlgorithm";
		}
	}
private:
	/**
	 * computes a target point based on the type of the robot
	 */
	Vector3d compute_target_point(const Vector3d & own_position, const std::vector<Vector3d>& positions);
	Type type_;
	boost::shared_ptr<DistributionGenerator> source_of_randomness;
};

#endif /* TP_ALGORITHM_ROBOT_H_ */
