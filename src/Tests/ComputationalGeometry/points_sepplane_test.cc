#include <boost/test/unit_test.hpp>

#include "../ComputationalGeometry/points_sepplane.h"

using namespace std;

BOOST_AUTO_TEST_CASE(separate_point_from_points_unit_cube_test) {
	//create points
	std::vector<Vector3d> points;
	Vector3d tmp_point;
	Vector3d check_point;
    Vector3d result;

    
	tmp_point(0) = 0;
	tmp_point(1) = 0;
	tmp_point(2) = 0;
	points.push_back(tmp_point);

	tmp_point(0) = 1;
	tmp_point(1) = 0;
	tmp_point(2) = 0;
	points.push_back(tmp_point);

	tmp_point(0) = 0;
	tmp_point(1) = 1;
	tmp_point(2) = 0;
	points.push_back(tmp_point);

	tmp_point(0) = 0;
	tmp_point(1) = 0;
	tmp_point(2) = 1;
	points.push_back(tmp_point);
    
    tmp_point(0) = 1;
	tmp_point(1) = 1;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
    tmp_point(0) = 0;
	tmp_point(1) = 1;
	tmp_point(2) = 1;
	points.push_back(tmp_point);
    
    tmp_point(0) = 1;
	tmp_point(1) = 0;
	tmp_point(2) = 1;
	points.push_back(tmp_point);
    
    tmp_point(0) = 1;
	tmp_point(1) = 1;
	tmp_point(2) = 1;
	points.push_back(tmp_point);

    check_point(0) = 0.5;
    check_point(1) = 0.5;
    check_point(2) = 0.5;
    BOOST_CHECK(separate_point_from_points(check_point, points) == boost::numeric::ublas::zero_vector<double>(3));
    
    // TODO(peter): following check currently failling due to a bug in convex hull algorithms
    check_point(0) = 0;
    check_point(1) = 0;
    check_point(2) = 0.77;
    BOOST_CHECK(separate_point_from_points(check_point, points) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = 0.1231;
    check_point(1) = 0.9713;
    check_point(2) = 0.34;
    BOOST_CHECK(separate_point_from_points(check_point, points) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = -0.5;
    check_point(1) =  0.5;
    check_point(2) =  0.5;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), 0.5);
    BOOST_CHECK_EQUAL(result(1), 0.0);
    BOOST_CHECK_EQUAL(result(2), 0.0);
    
    check_point(0) = -0.5;
    check_point(1) =  0.25;
    check_point(2) =  0.25;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), 0.5);
    BOOST_CHECK_EQUAL(result(1), 0.0);
    BOOST_CHECK_EQUAL(result(2), 0.0);
    
    check_point(0) = -10.5;
    check_point(1) =   0.25;
    check_point(2) =   0.25;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), 10.5);
    BOOST_CHECK_EQUAL(result(1),  0.0);
    BOOST_CHECK_EQUAL(result(2),  0.0);
    
    check_point(0) = -1;
    check_point(1) = -1;
    check_point(2) = -1;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 1);
    BOOST_CHECK_EQUAL(result(2), 1);
    
    check_point(0) = -0.1;
    check_point(1) = -1.0;
    check_point(2) = -0.1;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), 0.1);
    BOOST_CHECK_EQUAL(result(1), 1.0);
    BOOST_CHECK_EQUAL(result(2), 0.1);
    
    check_point(0) = 0;
    check_point(1) = 0;
    check_point(2) = -0.1;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0),  0.0);
    BOOST_CHECK_EQUAL(result(1),  0.0);
    BOOST_CHECK_EQUAL(result(2),  0.1);
    
    check_point(0) = 2;
    check_point(1) = 3;
    check_point(2) = 100;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0),  - 1);
    BOOST_CHECK_EQUAL(result(1),  - 2);
    BOOST_CHECK_EQUAL(result(2),  -99);
    
    check_point(0) = 0.5;
    check_point(1) = -0.1;
    check_point(2) = -0.1;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), 0.0);
    BOOST_CHECK_EQUAL(result(1), 0.1);
    BOOST_CHECK_EQUAL(result(2), 0.1);
}


BOOST_AUTO_TEST_CASE(separate_point_from_points_degenerated_line_test) {
	//create points
	std::vector<Vector3d> points;
	Vector3d tmp_point;
	Vector3d check_point;
    Vector3d result;
    
    
	tmp_point(0) = 0;
	tmp_point(1) = 0;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
	tmp_point(0) = 1;
	tmp_point(1) = 0;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
	tmp_point(0) = 2;
	tmp_point(1) = 0;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
    tmp_point(0) = 7;
	tmp_point(1) = 0;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
    check_point(0) = 1.2346;
    check_point(1) = 0;
    check_point(2) = 0;
    BOOST_CHECK(separate_point_from_points(check_point, points) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = 10;
    check_point(1) = 0;
    check_point(2) = 0;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), -3.0);
    BOOST_CHECK_EQUAL(result(1),  0.0);
    BOOST_CHECK_EQUAL(result(2),  0.0);
    
    check_point(0) = 1;
    check_point(1) = 1;
    check_point(2) = 1;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0),  0.0);
    BOOST_CHECK_EQUAL(result(1), -1.0);
    BOOST_CHECK_EQUAL(result(2), -1.0);
}


BOOST_AUTO_TEST_CASE(separate_point_from_points_degenerated_point_test) {
	//create points
	std::vector<Vector3d> points;
	Vector3d tmp_point;
	Vector3d check_point;
    Vector3d result;
    
    
	tmp_point(0) = 0;
	tmp_point(1) = 0;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
    check_point(0) = 0;
    check_point(1) = 0;
    check_point(2) = 0;
    BOOST_CHECK(separate_point_from_points(check_point, points) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = 1;
    check_point(1) = 1;
    check_point(2) = 1;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), -1);
    BOOST_CHECK_EQUAL(result(1), -1);
    BOOST_CHECK_EQUAL(result(2), -1);

    check_point(0) = 1;
    check_point(1) = 0;
    check_point(2) = 0;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0), -1);
    BOOST_CHECK_EQUAL(result(1),  0);
    BOOST_CHECK_EQUAL(result(2),  0);
    
    check_point(0) = -123;
    check_point(1) = -1.1309;
    check_point(2) = 12;
    result = separate_point_from_points(check_point, points);
    BOOST_CHECK_EQUAL(result(0),  123);
    BOOST_CHECK_EQUAL(result(1),  1.1309);
    BOOST_CHECK_EQUAL(result(2), -12);
}
