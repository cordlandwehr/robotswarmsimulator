/*
 * stats_control.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include <vector>

#include "../Model/robot_data.h"
#include "stats_control.h"

//StatsControl::StatsControl() {
//	stats_initialized_ = false;
//}
//
//StatsControl::~StatsControl() {
//	// TODO Auto-generated destructor stub
//}
//
//void StatsControl::init(Parser& parser) {
//	if (stats_initialized_) {
//		// log error, because no quit was called before this init
//		std::cerr << "StatsControl::init(...) called without any previous StatsControl::quit(...)" << std::endl;
//		// manually quit current statistic-calculation
//		quit();
//	}
//
//	// initialize StatsConfig
//	stats_cfg_.init(parser);
//
//	// abort if nothing to calculate
//	if (!stats_cfg_.is_any_subset())
//		return;
//
//	// TODO further initialization
//}
//
//void StatsControl::calculate() {
//	if (!stats_cfg_.is_any_subset())
//		return;
//
//	// Pointers to RobotData in the subsets might have changed,
//	// so recalculate all of them.
//	update_subsets();
//
//	if (stats_out_.size() != cur_subsets_.size())
//		std::cerr << "FAILURE: stats_out_.size() != cur_subsets_.size() in StatsControl.calculate()" << std::endl;
//
//
//}
//
//void StatsControl::update_subsets() {
//
//	// clear all subsets
//	for (int i=0; i<cur_subsets_.size(); i++) {
//		cur_subsets_[i].clear();
//	}
//	cur_subsets.clear();
//
//	// build new subsets
//	std::vector<boost::shared_ptr<RobotData> > all;
//	std::vector<boost::shared_ptr<RobotData> > actall;
//	std::vector<boost::shared_ptr<RobotData> > inactall;
//	std::vector<boost::shared_ptr<RobotData> > masters;
//	std::vector<boost::shared_ptr<RobotData> > actmasters;
//	std::vector<boost::shared_ptr<RobotData> > inactmasters;
//	std::vector<boost::shared_ptr<RobotData> > slaves;
//	std::vector<boost::shared_ptr<RobotData> > actslaves;
//	std::vector<boost::shared_ptr<RobotData> > inactslaves;
//
//	const std::vector<boost::shared_ptr<RobotData> >& robots = stats_calc_indata_.world_info_.robot_data();
//
//	for (int i=0; i<robots.size(); i++) {
//		boost::shared_ptr<RobotData> sp_robot = robots[i];
//
//		if (stats_cfg_.is_subset_all())
//			all.push_back(sp_robot);
//
//		if ((*sp_robot).type() == RobotType.MASTER) {
//			// type is MASTER
//			if (stats_cfg_.is_subset_masters())
//				masters.push_back(sp_robot);
//
//			if ((*sp_robot).status() == RobotStatus.READY) {
//				// status is READY
//				if (stats_cfg_.is_subset_actall())
//					actall.push_back(sp_robot);
//
//				if (stats_cfg_.is_subset_actmasters())
//					actmasters.push_back(sp_robot);
//
//			} else {
//				// status is SLEEPING
//				if (stats_cfg_.is_subset_inactall())
//					inactall.push_back(sp_robot);
//
//				if (stats_cfg_.is_subset_inactmasters())
//					inactmasters.push_back(sp_robot);
//			}
//		} else {
//			// type is SLAVE
//			if (stats_cfg_.is_subset_slaves())
//				slaves.push_back(sp_robot);
//
//			if ((*sp_robot).status() == RobotStatus.READY) {
//				// status is READY
//				if (stats_cfg_.is_subset_actall())
//					actall.push_back(sp_robot);
//
//				if (stats_cfg_.is_subset_actslaves())
//					actslaves.push_back(sp_robot);
//			} else {
//				// status is SLEEPING
//				if (stats_cfg_.is_subset_inactall())
//					inactall.push_back(sp_robot);
//
//				if (stats_cfg_.is_subset_inactslaves())
//					inactslaves.push_back(sp_robot);
//			}
//		}
//	}
//
//	// push subsets to cur_subsets
//	if (stats_cfg_.is_subset_all())
//		cur_subsets_.push_back(all);
//	if (stats_cfg_.is_subset_actall())
//		cur_subsets_.push_back(actall);
//	if (stats_cfg_.is_subset_inactall())
//		cur_subsets_.push_back(inactall);
//
//	if (stats_cfg_.is_subset_masters())
//		cur_subsets_.push_back(masters);
//	if (stats_cfg_.is_subset_actmasters())
//		cur_subsets_.push_back(actmasters);
//	if (stats_cfg_.is_subset_inactmasters())
//		cur_subsets_.push_back(inactmasters);
//
//	if (stats_cfg_.is_subset_slaves())
//		cur_subsets_.push_back(slaves);
//	if (stats_cfg_.is_subset_actslaves())
//		cur_subsets_.push_back(actslaves);
//	if (stats_cfg_.is_subset_inactslaves())
//		cur_subsets_.push_back(inactslaves);
//}
