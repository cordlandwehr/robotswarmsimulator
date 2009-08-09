#include <boost/test/unit_test.hpp>

#include "../ComputationalGeometry/ch_algorithms.h"
#include "../ComputationalGeometry/points_sepplane.h"


// This test makes sure that the CGAL kernel used does not cause infinite loops due to numerical rounding. If the unit
// tests deadlock here, it probably is caused by such a numeric instability.
BOOST_AUTO_TEST_CASE(separate_point_from_points_numeric_deadlock_test) {
    std::vector<Vector3d> points;
    
    Vector3d point;
    point(0) = -0.82978;
    point(1) = 4.97185;
    point(2) = 2.20324;
    points.push_back(point);
    
    point(0) = 4.32557;
    point(1) = -4.99886;
    point(2) = -3.71876;
    points.push_back(point);
    
    point(0) = 1.97667;
    point(1) = 4.99041;
    point(2) = -3.53244;
    points.push_back(point);
    
    point(0) = -2.63911;
    point(1) = -4.07661;
    point(2) = -1.03419;
    points.push_back(point);
    
    point(0) = -3.1374;
    point(1) = -1.12089;
    point(2) = -1.54439;
    points.push_back(point);
    
    point(0) = 1.69746;
    point(1) = -1.03233;
    point(2) = 4.35539;
    points.push_back(point);
    
    point(0) = 0.388167;
    point(1) = 3.46311;
    point(2) = -0.808055;
    points.push_back(point);
    
    CGAL::Object object;
    std::vector<Point_3> cgal_points = CHAlgorithms::transform(points);
    CGAL::convex_hull_3(cgal_points.begin(), cgal_points.end(), object);
}

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
    BOOST_CHECK(separate_point_from_points(check_point, points, 0.001) == boost::numeric::ublas::zero_vector<double>(3));
    
    // TODO(peter): following check currently failling due to a bug in convex hull algorithms
    check_point(0) = 0;
    check_point(1) = 0;
    check_point(2) = 0.77;
    BOOST_CHECK(separate_point_from_points(check_point, points, 0.001) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = 0.1231;
    check_point(1) = 0.9713;
    check_point(2) = 0.34;
    BOOST_CHECK(separate_point_from_points(check_point, points, 0.001) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = -0.5;
    check_point(1) =  0.5;
    check_point(2) =  0.5;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0.5);
    BOOST_CHECK_EQUAL(result(1), 0.0);
    BOOST_CHECK_EQUAL(result(2), 0.0);
    
    check_point(0) = -0.5;
    check_point(1) =  0.25;
    check_point(2) =  0.25;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0.5);
    BOOST_CHECK_EQUAL(result(1), 0.0);
    BOOST_CHECK_EQUAL(result(2), 0.0);
    
    check_point(0) = -10.5;
    check_point(1) =   0.25;
    check_point(2) =   0.25;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 10.5);
    BOOST_CHECK_EQUAL(result(1),  0.0);
    BOOST_CHECK_EQUAL(result(2),  0.0);
    
    check_point(0) = -1;
    check_point(1) = -1;
    check_point(2) = -1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 1);
    BOOST_CHECK_EQUAL(result(2), 1);
    
    check_point(0) = -0.1;
    check_point(1) = -1.0;
    check_point(2) = -0.1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0.1);
    BOOST_CHECK_EQUAL(result(1), 1.0);
    BOOST_CHECK_EQUAL(result(2), 0.1);
    
    check_point(0) = 0;
    check_point(1) = 0;
    check_point(2) = -0.1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0),  0.0);
    BOOST_CHECK_EQUAL(result(1),  0.0);
    BOOST_CHECK_EQUAL(result(2),  0.1);
    
    check_point(0) = 2;
    check_point(1) = 3;
    check_point(2) = 100;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0),  - 1);
    BOOST_CHECK_EQUAL(result(1),  - 2);
    BOOST_CHECK_EQUAL(result(2),  -99);
    
    check_point(0) = 0.5;
    check_point(1) = -0.1;
    check_point(2) = -0.1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0.0);
    BOOST_CHECK_EQUAL(result(1), 0.1);
    BOOST_CHECK_EQUAL(result(2), 0.1);
}


BOOST_AUTO_TEST_CASE(separate_point_from_points_degenerated_polyhedron_test) {
    // create points
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
    
	tmp_point(0) = 1;
	tmp_point(1) = 1;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
    // so far: triangle
    check_point(0) = 0.2;
    check_point(1) = 0.2;
    check_point(2) = 0;
    BOOST_CHECK(separate_point_from_points(check_point, points, 0.001) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = 0.2;
    check_point(1) = -0.2;
    check_point(2) = 0;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0);
    BOOST_CHECK_EQUAL(result(1), 0.2);
    BOOST_CHECK_EQUAL(result(2), 0);
    
    check_point(0) = 0.2;
    check_point(1) = 0.2;
    check_point(2) = 0.4;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0);
    BOOST_CHECK_EQUAL(result(1), 0);
    BOOST_CHECK_EQUAL(result(2), -0.4);
    
    check_point(0) = 0.2;
    check_point(1) = 0.2;
    check_point(2) = -0.4;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0);
    BOOST_CHECK_EQUAL(result(1), 0);
    BOOST_CHECK_EQUAL(result(2), 0.4);
    
    check_point(0) = 0.2;
    check_point(1) = -0.2;
    check_point(2) = 1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0);
    BOOST_CHECK_EQUAL(result(1), 0.2);
    BOOST_CHECK_EQUAL(result(2), -1);

    check_point(0) = 0.2;
    check_point(1) = -0.2;
    check_point(2) = -1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0);
    BOOST_CHECK_EQUAL(result(1), 0.2);
    BOOST_CHECK_EQUAL(result(2), 1);
    
    
    // now polyhedron, but planar
    tmp_point(0) = 0;
	tmp_point(1) = 1;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
    tmp_point(0) = 0.5;
	tmp_point(1) = 0.75;
	tmp_point(2) = 0;
	points.push_back(tmp_point);
    
    check_point(0) = 0.3;
	check_point(1) = 0.2;
	check_point(2) = 0;
    BOOST_CHECK(separate_point_from_points(check_point, points, 0.001) == boost::numeric::ublas::zero_vector<double>(3));

    check_point(0) = 0.3;
	check_point(1) = 0.2;
	check_point(2) = 0.1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0);
    BOOST_CHECK_EQUAL(result(1), 0);
    BOOST_CHECK_EQUAL(result(2), -0.1);
    
    check_point(0) = 0.3;
	check_point(1) = 0.2;
	check_point(2) = -0.1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 0);
    BOOST_CHECK_EQUAL(result(1), 0);
    BOOST_CHECK_EQUAL(result(2), 0.1);
    
    check_point(0) = -1;
	check_point(1) = 0.5;
	check_point(2) = 0.0;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 0);
    BOOST_CHECK_EQUAL(result(2), 0);
    
    check_point(0) = -1;
	check_point(1) = 0.5;
	check_point(2) = 1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 0);
    BOOST_CHECK_EQUAL(result(2), -1);
    
    check_point(0) = -1;
	check_point(1) = 0.5;
	check_point(2) = -1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 0);
    BOOST_CHECK_EQUAL(result(2), 1);
    
    check_point(0) = -1;
	check_point(1) = -0.5;
	check_point(2) = 0;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 0.5);
    BOOST_CHECK_EQUAL(result(2), 0);
    
    check_point(0) = -1;
	check_point(1) = -0.5;
	check_point(2) = -0.7;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 0.5);
    BOOST_CHECK_EQUAL(result(2), 0.7);
    
    check_point(0) = -1;
	check_point(1) = -0.5;
	check_point(2) = 0.7;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), 1);
    BOOST_CHECK_EQUAL(result(1), 0.5);
    BOOST_CHECK_EQUAL(result(2), -0.7);
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
    BOOST_CHECK(separate_point_from_points(check_point, points, 0.001) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = 10;
    check_point(1) = 0;
    check_point(2) = 0;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), -3.0);
    BOOST_CHECK_EQUAL(result(1),  0.0);
    BOOST_CHECK_EQUAL(result(2),  0.0);
    
    check_point(0) = 1;
    check_point(1) = 1;
    check_point(2) = 1;
    result = separate_point_from_points(check_point, points, 0.001);
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
    BOOST_CHECK(separate_point_from_points(check_point, points, 0.001) == boost::numeric::ublas::zero_vector<double>(3));
    
    check_point(0) = 1;
    check_point(1) = 1;
    check_point(2) = 1;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), -1);
    BOOST_CHECK_EQUAL(result(1), -1);
    BOOST_CHECK_EQUAL(result(2), -1);

    check_point(0) = 1;
    check_point(1) = 0;
    check_point(2) = 0;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0), -1);
    BOOST_CHECK_EQUAL(result(1),  0);
    BOOST_CHECK_EQUAL(result(2),  0);
    
    check_point(0) = -123;
    check_point(1) = -1.1309;
    check_point(2) = 12;
    result = separate_point_from_points(check_point, points, 0.001);
    BOOST_CHECK_EQUAL(result(0),  123);
    BOOST_CHECK_EQUAL(result(1),  1.1309);
    BOOST_CHECK_EQUAL(result(2), -12);
}
