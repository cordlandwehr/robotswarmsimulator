#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Statistics/vecset_stats.h"

#include "../../Utilities/vector_arithmetics.h"

BOOST_AUTO_TEST_CASE(vecset_stats_test) {
	// 1 or 0 whether or not print output to cout
	int kDebug = 0;

	// the vector-set-stats under test
	VecSetStats s = VecSetStats(VecSetStats::ALL);

	// test vector v = {(1,1,1),(2,2,2),(3,3,3), ...,(100,100,100)} (100 vectors)
	std::vector<boost::shared_ptr<Vector3d> > v;
	for (int i=0; i<100; i++) {
		boost::shared_ptr<Vector3d> vec = boost::shared_ptr<Vector3d>(new Vector3d());
		(*vec)[0] = i+1;
		(*vec)[1] = i+1;
		(*vec)[2] = i+1;
		v.push_back(vec);
	}

	s.handle(v);
	if (kDebug)
		std::cout << s.to_string() << std::endl;

	BOOST_CHECK_EQUAL(s.sum()[0], 5050);
	BOOST_CHECK_EQUAL(s.sum()[1], 5050);
	BOOST_CHECK_EQUAL(s.sum()[2], 5050);
	BOOST_CHECK_CLOSE(s.sumlen(), 8746.856578223, 0.00001);
	BOOST_CHECK_CLOSE(sqrt(s.sumnorm()[0]*s.sumnorm()[0]+s.sumnorm()[1]*s.sumnorm()[1]+s.sumnorm()[2]*s.sumnorm()[2]), 1.0, 0.00001);
	BOOST_CHECK_EQUAL(s.avg()[0], s.sum()[0]/100.0);
	BOOST_CHECK_EQUAL(s.avg()[1], s.sum()[1]/100.0);
	BOOST_CHECK_EQUAL(s.avg()[2], s.sum()[2]/100.0);
	BOOST_CHECK_CLOSE(s.avglen(), s.sumlen()/100.0, 0.00001);
	BOOST_CHECK_CLOSE(s.avgnorm()[0], s.sumnorm()[0], 0.00001);
	BOOST_CHECK_CLOSE(s.avgnorm()[1], s.sumnorm()[1], 0.00001);
	BOOST_CHECK_CLOSE(s.avgnorm()[2], s.sumnorm()[2], 0.00001);
	BOOST_CHECK_EQUAL(s.shortest()[0], 1);
	BOOST_CHECK_EQUAL(s.shortest()[1], 1);
	BOOST_CHECK_EQUAL(s.shortest()[2], 1);
	BOOST_CHECK_CLOSE(s.shortestlen(), sqrt(3.0), 0.00001);
	BOOST_CHECK_EQUAL(s.longest()[0], 100);
	BOOST_CHECK_EQUAL(s.longest()[1], 100);
	BOOST_CHECK_EQUAL(s.longest()[2], 100);
	BOOST_CHECK_CLOSE(s.longestlen(), sqrt(30000.0), 0.00001);
	BOOST_CHECK_CLOSE(s.cumullen(), 8746.856578223, 0.00001);
}
