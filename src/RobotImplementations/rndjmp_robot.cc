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

#include "../ComputationalGeometry/misc_algorithms.h"

#include "../ComputationalGeometry/miniball.h"
#include "../ComputationalGeometry/miniball.cc"
#include "../ComputationalGeometry/miniball_b.h"
#include "../ComputationalGeometry/miniball_b.cc"

using namespace std;

unsigned int RndJmpRobot::rndInit = 0;

std::set<boost::shared_ptr<Request> > RndJmpRobot::compute() {
		bool DEBUG = false;

		double v = view_->get_view_radius();
		double maxDist = 0.0; // not used atm

		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view_->get_visible_robots(*this);

		// create the vector of visible positions (in local coordinates, self in (0,0,0))
		std::vector<Vector3d> positions;

		positions.push_back(view_->get_position(*this, id()));

		// add all robots to positions and keep coordinates of nearest and farest.
		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			Vector3d q = view_->get_position(*this, cur_id);
			double curDist = vector3d_get_length(q, 2);
			if (curDist > maxDist)
				maxDist = curDist;
			positions.push_back(q);
		}

		// parameters for algorithm
		double r = m_/2.0;
		double alpha = alpha_;

		// calculate MinBall (center and diameter needed)
		Miniball<3> miniball;
		miniball.check_in(positions);
		miniball.build();
		double k = miniball.radius();

		// modify parameters in freeze-mode
		if (freeze_ > 0) {
			// calculate density t
			double t;

			t = positions.size() * s3_ / (freeze_ * 3.0/4.0 * k * k * k);

			if (t < eps_)
				t = eps_;
			if (t > 1.0 - eps_)
				t = 1.0 - eps_;

			if (log_)
				std::cout << "t = " << t << " (k=" << k << ")" << std::endl;

			r = (1 - t) * r;
			alpha = (1 + 3.0/4.0*t) * alpha;

			// reinit normal distribution with new alpha
			rand->init_normal(0, alpha);
		}

		// compute the most wanted target position
		Point<3> tppoint = miniball.center();
		Vector3d tp;
		tp[0] = tppoint[0];
		tp[1] = tppoint[1];
		tp[2] = tppoint[2];
		vector3d_set_maxlength(tp, r);

		// randomize the moving direction
		vector3d_rotate(tp, rand->get_value_normal(), rand->get_value_normal(), rand->get_value_normal());

		// cut move to keep visibilities
		MiscAlgorithms::cut_maxmove(tp, positions, v);

		// check if new position is safe
		double minDist = get_min_dist(tp, positions);

		// if not then stand still
		if (minDist < s_) {
			tp[0]=tp[1]=tp[2]=0;
		}

		// build position request and insert into request set
		boost::shared_ptr<Vector3d> result_ptr(new Vector3d(tp));
		boost::shared_ptr<PositionRequest> request(new PositionRequest(*this, result_ptr));
		std::set<boost::shared_ptr<Request> > result_set;
		result_set.insert(request);

		return result_set;
}

double RndJmpRobot::get_min_dist(Vector3d & tp, std::vector<Vector3d> & positions) {
	// assuming that robot itself with (0,0,0) is at positions[0]

	double minDist = vector3d_distance(tp, positions[1], 2);
	for (int i=2; i<positions.size(); i++) {
		double curDist = vector3d_distance(tp, positions[i], 2);
		if(curDist < minDist)
			minDist = curDist;
	}

	return minDist;
}
