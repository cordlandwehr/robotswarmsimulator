/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>

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
 * cog_robot.cc
 *
 *  Created on: Feb 27, 2009
 *      Author: craupach
 */

#include "pot_robot.h"
#include <cmath>

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../Requests/position_request.h"

#include "../Views/view.h"

#include "../Utilities/unsupported_operation_exception.h"
#include "../Utilities/vector_arithmetics.h"

#include "../ComputationalGeometry/point_algorithms.h"
#include "../ComputationalGeometry/misc_algorithms.h"

#include "../ComputationalGeometry/miniball.h"
#include "../ComputationalGeometry/miniball.cc"
#include "../ComputationalGeometry/miniball_b.h"
#include "../ComputationalGeometry/miniball_b.cc"

using namespace std;

unsigned int PotRobot::rndInit = 0;

std::set<boost::shared_ptr<Request> > PotRobot::compute() {
		boost::shared_ptr<View> view = view_;
		v_ = view_->get_view_radius();

		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);

		curMinDist_ = 999999.0;
		curMaxDist_ = 0.0;
		curAvgDist_ = 0.0;

		// create the vector of others' visible positions (in local coordinates)
		std::vector<Vector3d> others;
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			Vector3d q = view->get_position(*this, cur_id);
			others.push_back(q);

			double curDist = vector3d_get_length(q,2);

			if (curDist > curMaxDist_)
				curMaxDist_ = curDist;

			if (curDist < curMinDist_)
				curMinDist_ = curDist;

			curAvgDist_ += curDist;
		}

		curAvgDist_ /= others.size();

		// tp is new targetposition candidate
		Vector3d tp;

		// calculate potential for current position
		tp[0]=tp[1]=tp[2]=0;
		double origPot = calc_pot(tp, others);

		// calculate MinBall (center and diameter needed)
		Miniball<3> miniball;
		miniball.check_in(tp); // tp == 0 is self
		miniball.check_in(others);
		miniball.build();
		double k = miniball.radius();

		// try up to maxTries random positions
		int tryId = 1;
		bool found = false;
		while (!found && tryId<=maxTries_) {
			// compute a new random position
			std::vector<double> randVec = rand->get_value_uniform_on_sphere();

			tp[0] = randVec[0];
			tp[1] = randVec[1];
			tp[2] = randVec[2];

			double newLen;
			if (potfunc_id_ != 4) {
				newLen = 2.0*curAvgDist_*(rand->get_value_uniform())/100000.0;
				vector3d_set_length(tp, newLen);

			} else {
				rand->init_normal(0, 3.0/4.0*k);
				double r;
				do {
					r = rand->get_value_normal();
					r = abs(r);
				} while (r > k);

				vector3d_set_length(tp, r);

				Point<3> mbc = miniball.center();

				tp[0] = mbc[0] + tp[0];
				tp[1] = mbc[1] + tp[1];
				tp[2] = mbc[2] + tp[2];
			}

			// cut move to keep visibilities
			MiscAlgorithms::cut_maxmove(tp, others, v_);

			// compute potential over there
			double newPot = calc_pot(tp, others);

			if (potfunc_id_ == 4 && newPot == origPot) {
				// special case for potfunc 4
				found = true;
			}

			if (newPot < origPot) {
				found = true;
				if (LOGINFO)
					std::cout << newPot << " < " << origPot
							<< " at distance " << vector3d_get_length(tp, 2)
							<< " (max. was " << newLen
							<< ") in " << tryId << ". try"
							<< std::endl;
			}

			tryId++;
		}

		// create and return request
		std::set<boost::shared_ptr<Request> > result_set;
		if (found) {
			// build position request
			boost::shared_ptr<Vector3d> result_ptr(new Vector3d(tp));
			boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, result_ptr));

			// insert request into request set
			result_set.insert(request);
		} else {
			if (LOGINFO)
				std::cout << "found no position" << std::endl;
		}

		return result_set;
}

double PotRobot::calc_pot(Vector3d & me, std::vector<Vector3d> & others) {
	switch (potfunc_id_) {
		case 1 :
			return calc_pot_1(me, others);
		case 2 :
			return calc_pot_2(me, others);
		case 3 :
			return calc_pot_3(me, others);
		case 4 :
			return calc_pot_4(me, others);
		default:
			throw UnsupportedOperationException("Given potential function ID is unknown.");
	}
}

double PotRobot::calc_pot_1(Vector3d & me, std::vector<Vector3d> & others) {
	double res = 0.0;
	double dx, dy, dz;
	BOOST_FOREACH(Vector3d other, others) {
		dx = me[0] - other[0];
		dy = me[1] - other[1];
		dz = me[2] - other[2];
		double curDist = dx*dx + dy*dy + dz*dz;
		curDist = sqrt(curDist);
		if (curDist < finalMinDist_)
			res += 999999;
		else
			res += pow(curDist - aimDist_, 2);
	}

	return res;
}

double PotRobot::calc_pot_2(Vector3d & me, std::vector<Vector3d> & others) {
	double dx, dy, dz;
	double curMinDist = 9999999;
	double curMaxDist = 0;

	BOOST_FOREACH(Vector3d other, others) {
		dx = me[0] - other[0];
		dy = me[1] - other[1];
		dz = me[2] - other[2];
		double curDist = dx*dx + dy*dy + dz*dz;
		curDist = sqrt(curDist);
		if (curDist < curMinDist)
			curMinDist = curDist;
		if (curDist > curMaxDist)
			curMaxDist = curDist;
	}

	if (curMinDist < finalMinDist_)
		return 999999;
	else
		return curMaxDist;
}

double PotRobot::calc_pot_3(Vector3d & me, std::vector<Vector3d> & others) {
	double res = 0.0;
	double dx, dy, dz;
	BOOST_FOREACH(Vector3d other, others) {
		dx = me[0] - other[0];
		dy = me[1] - other[1];
		dz = me[2] - other[2];
		double curDist = dx*dx + dy*dy + dz*dz;
		curDist = sqrt(curDist);

		if (curDist < finalMinDist_)
			res += 999999;
		else
			if (curDist < aimDist_/2.0 || curDist > 2.0 * aimDist_)
				res += 1;
	}

	return res;
}

double PotRobot::calc_pot_4(Vector3d & me, std::vector<Vector3d> & others) {
	double dx, dy, dz;
	BOOST_FOREACH(Vector3d other, others) {
		dx = me[0] - other[0];
		dy = me[1] - other[1];
		dz = me[2] - other[2];
		double curDist = dx*dx + dy*dy + dz*dz;
		curDist = sqrt(curDist);
		if (curDist < finalMinDist_)
			return 999999;
	}

	// calculate MinBall (only diameter needed)
	Miniball<3> miniball;
	miniball.check_in(me);
	miniball.check_in(others);
	miniball.build();
	return miniball.radius();
}
