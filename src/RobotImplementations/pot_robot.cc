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

#include "../Utilities/vector_arithmetics.h"

#include "../ComputationalGeometry/point_algorithms.h"

using namespace std;

void PotRobot::cut_trivial(Vector3d & tp, std::vector<Vector3d> & positions, double v) {
	// calculate maximal distance from self (position[0]) to any other (=> i>0) robot
	double maxDist = 0.0;
	for (int i=1; i<positions.size(); i++) {
		double dist = vector3d_distance(positions[0], positions[i]);
		if (dist > maxDist) {
			maxDist = dist;
		}
	}

	// cut if maxDist too big
	maxDist = (v-maxDist)/2.0;
	vector3d_set_maxlength(tp, maxDist);
}

void PotRobot::cut_maxmove(Vector3d & tp, std::vector<Vector3d> & positions, double v) {
	bool DEBUG = false;

	// for each other robot do
	for (int i=1; i<positions.size(); i++) {
		Vector3d q = positions[i];

		// calculate distance of target tp and middlepos of me and other q/2
		//if ((vector3d_get_length(q, 2) <= v) && (vector3d_distance(tp, q/2.0) > v/2.0)) {
		if (vector3d_distance(tp, q/2.0) > v/2.0) {
			// calculate some magic a,b,c (from maths behind)
			double a, b, c;
			a = tp[0]*tp[0] + tp[1]*tp[1] + tp[2]*tp[2];
			b = -q[0]*tp[0]-q[1]*tp[1]-q[2]*tp[2];
			c = (q[0]*q[0] + q[1]*q[1] + q[2]*q[2] - v*v)/4.0;

			if (b*b-4*a*c < 0) {
				cout << "ERROR: 0 > b*b-4*a*c = " << b*b-4*a*c << endl;
				cout << " for tp=" << tp << " q= " << q << " (self at=" << positions[0] << ")" << endl;
			}

			// and calculate both solutions of a * t^2 + b*t + c = 0
			double t1 = (-b - sqrt(b*b-4*a*c))/(2*a);
			double t2 = (-b + sqrt(b*b-4*a*c))/(2*a);

			// we might assert: Either t1=t2=0
			// or there exists exactly one tj with tj in ]0,1[ and the other is < 0
			// we use the bigger to shorten our tp nearer to self position
			double t = max(t1, t2);

			if (abs(t) < 0.01)
				t = 0;

			if (t < 0) {
				cout << "ERROR: t1=" << t1 << ", t2=" << t2;
				cout << " for tp=" << tp << " q= " << q << " (self at=" << positions[0] << ")" << endl;
			}

			if (DEBUG)
				cout << "old tp=" << tp;

			tp = t * tp;

			/*if ((vector3d_distance(tp, q/2.0) > v/2.0 - 0.001)
					&& (vector3d_distance(tp, q) >= vector3d_get_length(q, 2))) {
				double tplen = vector3d_get_length(tp, 2);
				// cut if we increase distance *and* distance is already very far
				cout << "cutting... from |tp|=" << tplen << endl;
				if (tplen < 0.1)
					tp[0] = tp[1] = tp[2] = 0;
				else
					vector3d_set_length(tp, tplen-0.1);
			}*/

			if (DEBUG) {
				cout << "a,b,c=" << a << ", " << b << ", " << c << " ";
				cout << " new tp=" << tp << " (by t=" << t << ")" << endl;
			}
		}
	}

	double maxDist = 0;
	for (int i=1; i<positions.size(); i++) {
		maxDist = max(maxDist, vector3d_distance(tp, positions[i]));
	}
	//cout << "newMaxDist=" << maxDist << endl;
}

std::set<boost::shared_ptr<Request> > PotRobot::compute() {
		boost::shared_ptr<View> view = view_;
		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);

		curMinDist_ = 999999.0;
		curMaxDist_ = 0.0;
		curAvgDist_ = 0.0;

		// create the vector of others' visible positions (in local coordinates)
		std::vector<Vector3d> others;
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			Vector3d q = view->get_position(*this, cur_id);
			double curDist = vector3d_get_length(q,2);

			if (curDist <= v_)
				others.push_back(q);

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

		// try up to maxTries random positions
		int tryId = 1;
		bool found = false;
		while (!found && tryId<=maxTries_) {
			// compute a new random position
			std::vector<double> randVec = rand->get_value_uniform_on_sphere();
			tp[0] = randVec[0];
			tp[1] = randVec[1];
			tp[2] = randVec[2];
			double newLen = 2.0*curAvgDist_*(rand->get_value_uniform())/100000.0;
			vector3d_set_length(tp, newLen);

			// cut position by maxmove
			cut_maxmove(tp, others, v_);

			// compute potential over there
			double newPot = calc_pot(tp, others);

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
			return calc_pot_A(me, others);
		case 2 :
			return calc_pot_B(me, others);
		case 3 :
			return calc_pot_C(me, others);
	}
}

double PotRobot::calc_pot_A(Vector3d & me, std::vector<Vector3d> & others) {
	double res = 0.0;
	double dx, dy, dz;
	double curMinDist = 9999999;
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

double PotRobot::calc_pot_B(Vector3d & me, std::vector<Vector3d> & others) {
	double res = 0.0;
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

double PotRobot::calc_pot_C(Vector3d & me, std::vector<Vector3d> & others) {
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
