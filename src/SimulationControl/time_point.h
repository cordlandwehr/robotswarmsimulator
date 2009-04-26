/*
 * time_point.h
 *
 *  Created on: Apr 26, 2009
 *      Author: craupach
 */

#ifndef TIME_POINT_H_
#define TIME_POINT_H_

#include "../Model/world_information.h"
#include <boost/smart_ptr.hpp>

/**
 * The TimePoint class encapusaltes a WorldInformation Object and statistical information for a certain
 * point in time.
 */
class TimePoint {
public:
	TimePoint() : locked_(false) {}
	/**
	 * copy constructor copies the world information and the statistical data
	 * Note that a copy will always be locked (no modifications possible)
	 */
	TimePoint(const TimePoint& rhs) : world_information_(new WorldInformation(rhs.world_information())),
	                                  locked_(true) {};

	void set_world_information(boost::shared_ptr<WorldInformation> world_information) {
		if(!locked_) {
			world_information_ = world_information;
		}
	}

	const boost::shared_ptr<WorldInformation> world_information_ptr() const {return world_information_;}
	const WorldInformation& world_information() const {return *world_information_;}

	/**
	 * checks if this time point represents a real WorldInformation object
	 */
	bool isValid() {return world_information_;}

	/**
	 * locks the time point
	 */
	void lock() { locked_ = true;}
private:
	boost::shared_ptr<WorldInformation> world_information_;

	/**
	 * true iff no further modifications should be allowed.
	 */
	bool locked_;
};

#endif /* TIME_POINT_H_ */
