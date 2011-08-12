/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * asynchronous_asg_test_accessor.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef ASYNCHRONOUS_ASG_TEST_ACCESSOR_H_
#define ASYNCHRONOUS_ASG_TEST_ACCESSOR_H_

#include "../../ActivationSequenceGenerators/asynchronous_asg.h"
#include <boost/smart_ptr.hpp>
#include <list>

/**
 * This Accessor allows us to see private data of the contained asynchronous asg.
 */
class AsynchronousASGTestAccessor {
public:
	AsynchronousASGTestAccessor(AsynchronousASG& asg) : asg_(asg) {}

	/**
	 * accessor for the unhandled request set
	 */
	const std::list<boost::shared_ptr<const Request> >& unhandled_request_set() {
		return asg_.unhandled_request_set_;
	}

	/**
	 * accessor for looking robots
	 */
	const std::list<boost::shared_ptr<Robot> >& looking_robots() {
		return asg_.looking_robots_;
	}

	/**
	 * accessor for computing robots
	 */
	const std::list<boost::shared_ptr<Robot> >& computing_robots() {
		return asg_.computing_robots_;
	}

	/**
	 * accessor for handling robots
	 */
	const std::list<boost::shared_ptr<Robot> >& handling_robots() {
		return asg_.handling_robots_;
	}

	/**
	 * returns the sum of all robots in the lists
	 */
	int add_list_sizes() {
		return asg_.looking_robots_.size() +
		       asg_.computing_robots_.size() +
		       asg_.handling_robots_.size();
	}

private:
	/**
	 * the asg to be tested
	 */
	AsynchronousASG& asg_;
};

#endif
