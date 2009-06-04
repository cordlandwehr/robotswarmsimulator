/*
 * cog_robot.cc
 *
 *  Created on: Feb 27, 2009
 *      Author: craupach
 */

#include "rndjmp_robot.h"
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

void RndJmpRobot::cut_trivial(Vector3d & tp, std::vector<Vector3d> & positions, double v) {
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

void RndJmpRobot::cut_maxmove(Vector3d & tp, std::vector<Vector3d> & positions, double v) {
	bool DEBUG = true;

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
	cout << "newMaxDist=" << maxDist << endl;
}

std::set<boost::shared_ptr<Request> > RndJmpRobot::compute() {
		// TODO atm very bad: the robot's view-radius is hardcoded, because I don't know how to read it.
		double v = 9;
		bool DEBUG = false;

		boost::shared_ptr<View> view = view_;
		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view->get_visible_robots(*this);

		// create the vector of visible positions (in local coordinates, self in (0,0,0))
		std::vector<Vector3d> positions;

		positions.push_back(view->get_position(*this, id()));

		double maxDist = 0;
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			Vector3d q = view->get_position(*this, cur_id);
			if (vector3d_get_length(q, 2) < 9.01) {
				vector3d_set_maxlength(q, 9);
				maxDist = max(maxDist, vector3d_get_length(q, 2));
				positions.push_back(q);
			} else {
				cout << "too far" << endl;
			}
		}
		cout << "maxDist=" << maxDist << endl;

		// compute the main target position
		Vector3d tp = PointAlgorithms::compute_CMinBox(positions);
		vector3d_set_maxlength(tp, v);

		//tp[0] += rand->get_value_normal();
		//tp[1] += rand->get_value_normal();
		//tp[2] += rand->get_value_normal();

		//vector3d_set_length(tp, 1);

		cut_maxmove(tp, positions, v);

		// build position request
		boost::shared_ptr<Vector3d> result_ptr(new Vector3d(tp));
		boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, result_ptr));

		// insert request into request set
		std::set<boost::shared_ptr<Request> > result_set;
		result_set.insert(request);

		return result_set;
}
