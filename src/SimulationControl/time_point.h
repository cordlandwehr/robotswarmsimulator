/*
 * time_point.h
 *
 *  Created on: Apr 26, 2009
 *      Author: craupach
 */

#ifndef TIME_POINT_H_
#define TIME_POINT_H_

#include "../Model/world_information.h"
#include "../Statistics/statistics_data_object.h"
#include <boost/smart_ptr.hpp>

#include <iostream>

/**
 * The TimePoint class encapusaltes a WorldInformation Object and statistical information for a certain
 * point in time.
 * A TimePoint in the History MUST contain a WorldInformation. It MAY contain statistical information.
 */
class TimePoint {
public:
	TimePoint() :/* statistics_locked_(false), */world_information_locked_(false) {}
	/**
	 * copy constructor copies the world information and the statistical data
	 * Note that a copy will always be locked (no modifications possible)
	 */
	TimePoint(const TimePoint& rhs) : world_information_(new WorldInformation(rhs.world_information())),
	                                 /* statistics_locked_(true),*/
	                                  world_information_locked_(true) {
		/*if(rhs.statistics_data_object_ptr()) {
			statistics_data_object_ =
			    boost::shared_ptr<StatisticsDataObject> (new StatisticsDataObject(rhs.statistics_data_object()));
		}*/
	};

	/**
	 * Inserts a new world information and locks the world information part of the time point
	 */
	void set_world_information(boost::shared_ptr<WorldInformation> world_information) {
		if(!world_information_locked_) {
			world_information_ = world_information;
			world_information_locked_ = true;
		}
	}

	/*/**
	 * Inserts a new statistics object and locks the statistics part of the time point
	 */
	/*void set_statistics_data_object(boost::shared_ptr<StatisticsDataObject> stat_object) {
		if(!statistics_locked_) {
			statistics_data_object_ = stat_object;
			statistics_locked_ = true;
		}
	}*/

	const boost::shared_ptr<WorldInformation> world_information_ptr() const {return world_information_;}
	const WorldInformation& world_information() const {return *world_information_;}

	//const StatisticsDataObject& statistics_data_object() const {return *statistics_data_object_;}
	//const boost::shared_ptr<StatisticsDataObject> statistics_data_object_ptr() const {return statistics_data_object_;}

	/**
	 * checks if this time point represents a real WorldInformation object.
	 * There may be a statistics object but not necessarly
	 */
	bool isValid() {return world_information_;}

	/**
	 * locks the time point
	 */
	void lock() {
		world_information_locked_ = true;
		/*statistics_locked_ = true;*/
	}
private:
	boost::shared_ptr<WorldInformation> world_information_;
	//boost::shared_ptr<StatisticsDataObject> statistics_data_object_;

	/**
	 * true iff no further modifications should be allowed. This is set to true as soon as
	 * a statistics object is inserted or the time point is inserted into the history.
	 */
	//bool statistics_locked_;

	/**
	 * true iff no further modification of the world information should be allowed.
	 * This is set to true as soon as the world information is inserted.
	 */
	bool world_information_locked_;


};

#endif /* TIME_POINT_H_ */
