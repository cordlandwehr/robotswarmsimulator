#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include <iostream>
#include <vector>
#include <math.h>

#include "../../Statistics/numset_stats.h"
#include "../../Model/robot.h"

#include "../../Utilities/vector_arithmetics.h"

BOOST_AUTO_TEST_CASE(numset_stats_test) {
	// 1 or 0 whether or not print output to cout
	int kDebug = 0;

	// the number-set-stats under test
	NumSetStats s = NumSetStats(NumSetStats::ALL);

	// test vector v = {42, 42, ..., 42} (100 times)
	std::vector<double> v;
	v.assign(100, 42);

	s.handle(v);
	if (kDebug)
		std::cout << s.to_string() << std::endl;

	BOOST_CHECK_EQUAL(s.min(), 42);
	BOOST_CHECK_EQUAL(s.max(), 42);
	BOOST_CHECK_EQUAL(s.diam(), 0);
	BOOST_CHECK_EQUAL(s.avg(), 42);
	BOOST_CHECK_EQUAL(s.sum(), 4200);
	BOOST_CHECK_EQUAL(s.abssum(), 4200);
	BOOST_CHECK_EQUAL(s.median(), 42);
	BOOST_CHECK_EQUAL(s.stddeviation(), 0);


	// test vector v = {-1, 0.5, 1, -2, 0.5, 2}
	v.clear();
	v.push_back(-1);
	v.push_back(0.5);
	v.push_back(1);
	v.push_back(-2);
	v.push_back(2);
	v.push_back(-0.5);

	s.handle(v);
	if (kDebug)
		std::cout << s.to_string() << std::endl;

	BOOST_CHECK_EQUAL(s.min(), -2);
	BOOST_CHECK_EQUAL(s.max(), 2);
	BOOST_CHECK_EQUAL(s.diam(), 4);
	BOOST_CHECK_EQUAL(s.avg(), 0);
	BOOST_CHECK_EQUAL(s.sum(), 0);
	BOOST_CHECK_EQUAL(s.abssum(), 7);
	BOOST_CHECK_EQUAL(s.median(), -0.5);
	BOOST_CHECK_CLOSE(s.stddeviation(), sqrt(1.75), 0.00001);
}
