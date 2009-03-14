/*
 * stats_calc.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include "stats_calc.h"
#include "../Model/robot_data.h"
#include "../Utilities/miniball.h"
#include "../Utilities/miniball.cc"
#include "../Utilities/miniball_b.cc"
#include "../Utilities/vector_arithmetics.h"

StatsCalc::StatsCalc() {

}

StatsCalc::~StatsCalc() {

}

void StatsCalc::init(StatsConfig* stats_cfg) {
	this->stats_cfg = stats_cfg;
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
	for (unsigned int i=0; i<subset.size(); i++)
		vector3d_set((*positions.get())[i], subset[i]->position());

	bool push_names = !(stats_out->is_open());

	if (DEBUG)
		std::cout << "DEBUG: >>>> pushing " << push_names << std::endl ;


	if (stats_cfg->is_num_robots()) {
		// log number of robots
		values.push_back(subset.size());
		if (push_names)
			names.push_back("num_robots");
	}

	if (stats_cfg->is_num_masters() || stats_cfg->is_num_slaves()) {
		int num_masters=0;
		int num_slaves=0;

		for (unsigned int i=0; i<subset.size(); i++) {
			if (subset[i]->type() == MASTER)
				num_masters++;
			else
				num_slaves++;
		}

		if (stats_cfg->is_num_masters()) {
			// log num_masters
			values.push_back(num_masters);
			if (push_names)
				names.push_back("num_masters");
		}
		if (stats_cfg->is_num_slaves()) {
			// log num_slaves
			values.push_back(num_slaves);
			if (push_names)
				names.push_back("num_slaves");
		}
	}

	if (stats_cfg->is_miniball_center() || stats_cfg->is_miniball_radius() || stats_cfg->is_miniball_movedist()) {
		// calculate the miniball
		Miniball<3> mb;
		mb.check_in(*positions.get());

		mb.build();

		if (stats_cfg->is_miniball_center()) {
			values.push_back(mb.center()[0]);
			values.push_back(mb.center()[1]);
			values.push_back(mb.center()[2]);
			if (push_names) {
				names.push_back("miniball_center_x");
				names.push_back("miniball_center_y");
				names.push_back("miniball_center_z");
			}
		}

		if (stats_cfg->is_miniball_radius()) {
			values.push_back(mb.radius());
			if (push_names)
				names.push_back("miniball_radius");
		}

		if (stats_cfg->is_miniball_movedist()) {
			double miniball_movedist=0.0;
			if (data.prev_world_info_.get() != NULL) {
				miniball_movedist = mb.center().dist(*data.prev_miniball_center[subset_id]);
			}
			values.push_back(miniball_movedist);
			if (push_names)
				names.push_back("miniball_movedist");
		}

		mb.center().set_other(*data.prev_miniball_center[subset_id]);
		data.prev_miniball_radius[subset_id] = mb.radius();
	}

//	if (stats_cfg->vel_cfg() != 0) {
//		// log velocity-information
//		tmp_numstats_vector.clear();
//		for(unsigned int i=0; i<subset.size(); i++) {
//			//"tmp_numstats_vector.push_back(subset[i].get()->velocity_len());"
//		}
//
//		num_stats.handle(tmp_numstats_vector, stats_cfg->vel_cfg());
//		num_stats.push_values(values);
//		if (push_names)
//			num_stats.push_names(names, "vel");
//	}

	// DO ALL THE CALCULATION
	// ...

	data.prev_positions[subset_id] = positions;

	if (DEBUG && push_names)
		std::cout << "DEBUG:      opening StatsOut with " << names.size() << " names." << std::endl;

	if (push_names)
		(*stats_out).open(names);

	if (DEBUG)
		std::cout << "DEBUG:      calculation done. Now updating logfiles..." << std::endl;

	stats_out->update(data.world_info_->time(), values);

	if (DEBUG)
		std::cout << "DEBUG: <<<<" << std::endl;
}
