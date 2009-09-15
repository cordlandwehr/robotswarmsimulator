/*
 * stats_calc.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include <cmath>

#include "stats_calc.h"
#include "float.h"

#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../ComputationalGeometry/miniball.h"
#include "../ComputationalGeometry/miniball.cc"
#include "../ComputationalGeometry/miniball_b.cc"
#include "../Utilities/vector_arithmetics.h"
#include "../Views/view.h"

StatsCalc::StatsCalc() {

}

StatsCalc::~StatsCalc() {

}

void StatsCalc::init(StatsConfig* stats_cfg) {
	this->stats_cfg_ = stats_cfg;
}

void StatsCalc::calculate(StatsCalcInData & data,
		std::vector<boost::shared_ptr<RobotData> > & subset,
		boost::shared_ptr<StatsOut> & stats_out,
		unsigned int subset_id) {

	if (DEBUG)
		std::cout << "DEBUG: >>>> StatsCalc::calculate(...) with " << subset.size() << " robots." << std::endl;

	std::vector<std::string> names;
	std::vector<double> values;
	std::vector<double> tmp_numstats_vector;

	boost::shared_ptr<std::vector<Vector3d> > positions = boost::shared_ptr<std::vector<Vector3d> >(new std::vector<Vector3d>(subset.size()));
	for (unsigned int i = 0; i < subset.size(); i++)
		vector3d_set((*positions.get())[i], subset[i]->position());

	bool push_names = !(stats_out->is_open());

	if (DEBUG)
		std::cout << "DEBUG: >>>> pushing " << push_names << std::endl ;


	if (stats_cfg_->is_num_robots()) {
		// log number of robots
		values.push_back(subset.size());
		if (push_names)
			names.push_back("num_robots");
	}

	if (stats_cfg_->is_num_masters() || stats_cfg_->is_num_slaves()) {
		int num_masters = 0;
		int num_slaves = 0;

		for (unsigned int i = 0; i < subset.size(); i++) {
			if (subset[i]->type() == MASTER)
				num_masters++;
			else
				num_slaves++;
		}

		if (stats_cfg_->is_num_masters()) {
			// log num_masters
			values.push_back(num_masters);
			if (push_names)
				names.push_back("num_masters");
		}
		if (stats_cfg_->is_num_slaves()) {
			// log num_slaves
			values.push_back(num_slaves);
			if (push_names)
				names.push_back("num_slaves");
		}
	}

	if (stats_cfg_->is_miniball_center() || stats_cfg_->is_miniball_radius() ||
	    stats_cfg_->is_miniball_movedist() || stats_cfg_->is_volume_quot()) {
		// calculate the miniball
		Miniball<3> mb;
		mb.check_in(*positions.get());

		mb.build();

		if (stats_cfg_->is_miniball_center()) {
			values.push_back(mb.center()[0]);
			values.push_back(mb.center()[1]);
			values.push_back(mb.center()[2]);
			if (push_names) {
				names.push_back("miniball_center_x");
				names.push_back("miniball_center_y");
				names.push_back("miniball_center_z");
			}
		}

		if (stats_cfg_->is_miniball_radius()) {
			values.push_back(mb.radius());
			if (push_names)
				names.push_back("miniball_radius");
		}

		if (stats_cfg_->is_miniball_movedist()) {
			double miniball_movedist=0.0;
			if (data.prev_world_info_.get() != NULL) {
				miniball_movedist = mb.center().dist(*data.prev_miniball_center[subset_id]);
			}
			values.push_back(miniball_movedist);
			if (push_names)
				names.push_back("miniball_movedist");
		}

		if (stats_cfg_->is_volume_quot()) {
			double volRobots = subset.size() * 0.002197; // = 0.13^3
			double volMinball = mb.radius() * mb.radius() * mb.radius();
			if (volMinball == 0)
				values.push_back(-1);
			else
				values.push_back(volRobots / volMinball);

			if (push_names)
				names.push_back("volume_quot");
		}

		mb.center().set_other(*data.prev_miniball_center[subset_id]);
		data.prev_miniball_radius[subset_id] = mb.radius();
	}

	if (stats_cfg_->is_visgraph_connected()) {
		values.push_back(data.visib_->vis_graph_is_connected());
		if (push_names)
			names.push_back("visgraph_connected");
	}

	if (stats_cfg_->is_max_mindist()) {
		double max_minDist = 0.0;

		for (unsigned int i = 0; i < positions->size(); i++) {
			double curMinDist = DBL_MAX;

			for (unsigned int j = 0; j < positions->size(); j++) {
				if (i == j)
					continue;

				double curDist = vector3d_distance(positions->at(i), positions->at(j), 2);

				if (curDist < curMinDist)
					curMinDist = curDist;
			}

			if (curMinDist > max_minDist)
				max_minDist = curMinDist;
		}

		values.push_back(max_minDist);
		if (push_names)
			names.push_back("max_minDist");
	}

	if (stats_cfg_->is_max_origindist()) {
		double maxDist = 0.0;
		for (unsigned int j = 0; j < positions->size(); j++) {
			double curDist = vector3d_get_length(positions->at(j), 2);
			if (curDist > maxDist)
				maxDist = curDist;
		}

		values.push_back(maxDist);
		if (push_names)
			names.push_back("max_originDist");
	}

    if (stats_cfg_->is_max_l1origindist()) {
        double maxDist = 0.0;
		for (unsigned int j = 0; j < positions->size(); j++) {
			double curDist = std::abs(positions->at(j)(0)) + std::abs(positions->at(j)(1)) + std::abs(positions->at(j)(2));
			if (curDist > maxDist)
				maxDist = curDist;
		}

		values.push_back(maxDist);
		if (push_names)
			names.push_back("max_l1originDist");
    }

	// DO ALL THE CALCULATION
	// ...

	data.prev_positions[subset_id] = positions;

	if (DEBUG && push_names)
		std::cout << "DEBUG:      opening StatsOut with " << names.size() << " names." << std::endl;

	if (push_names)
		stats_out->open(names);

	if (DEBUG)
		std::cout << "DEBUG:      calculation done. Now updating logfiles..." << std::endl;

	stats_out->update(data.world_info_->time(), values);

	if (DEBUG)
		std::cout << "DEBUG: <<<<" << std::endl;
}
