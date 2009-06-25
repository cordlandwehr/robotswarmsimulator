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

using namespace std;

unsigned int RndJmpRobot::rndInit = 0;

std::set<boost::shared_ptr<Request> > RndJmpRobot::compute() {
		bool DEBUG = false;

		double s = 0.25;

		double v = view_->get_view_radius();

		std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots = view_->get_visible_robots(*this);

		// create the vector of visible positions (in local coordinates, self in (0,0,0))
		std::vector<Vector3d> positions;

		positions.push_back(view_->get_position(*this, id()));

		double maxDist = 0;
		double minXYZ[3];
		double maxXYZ[3];

		minXYZ[0]=minXYZ[1]=minXYZ[2]=999999999;
		maxXYZ[0]=maxXYZ[1]=maxXYZ[2]=0;

		BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
			Vector3d q = view_->get_position(*this, cur_id);
			maxDist = max(maxDist, vector3d_get_length(q, 2));
			positions.push_back(q);
			for (int d=0; d<3; d++) {
				if (q[d] < minXYZ[d])
					minXYZ[d] = q[d];
				if (q[d] > maxXYZ[d])
					maxXYZ[d] = q[d];
			}
		}

		double diam = 0.0;
		for (int d=0; d<3; d++)
			diam += (maxXYZ[d]-minXYZ[d])*(maxXYZ[d]-minXYZ[d]);
		diam = sqrt(diam);

		if (DEBUG) {
			cout << "diam=" << diam << endl;
			cout << "maxDist=" << maxDist << endl;
		}

		// compute the main target position
		Vector3d tp = PointAlgorithms::compute_CMinBox(positions);

		// set most wanted target position
		vector3d_set_maxlength(tp, s/2.0);

		// trim further by density
		double densityTrim = diam / positions.size();
		vector3d_set_maxlength(tp, densityTrim);

		// randomize the moving direction
		vector3d_rotate(tp, rand->get_value_normal(), rand->get_value_normal(), rand->get_value_normal());

		// cut move to keep visibilities
		MiscAlgorithms::cut_maxmove(tp, positions, v);
		//MiscAlgorithms::cut_trivial(tp, positions, v);

		// check if new position is safe
		double minDist = get_min_dist(tp, positions);

		// if not then stand still
		if (minDist < s)
			tp[0]=tp[1]=tp[2]=0;

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
