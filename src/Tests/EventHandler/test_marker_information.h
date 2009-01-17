/*
 * test_marker_information.h
 *
 *  Created on: 17.01.2009
 *      Author: peter
 */

#ifndef TEST_MARKER_INFORMATION_H_
#define TEST_MARKER_INFORMATION_H_

#include <boost/shared_ptr.hpp>

#include "../../Model/marker_information.h"

/**
 * Simple marker information carrying an integral (non-mutable) information.
 */
class TestMarkerInformation : public MarkerInformation {
public:
	TestMarkerInformation(int value) : value_(value) { } ;

	boost::shared_ptr<MarkerInformation> clone() const {
		return boost::shared_ptr<MarkerInformation>(new TestMarkerInformation(*this));
	}

	int value() const { return value_; };

private:
	const int value_;
};

#endif /* TEST_MARKER_INFORMATION_H_ */
