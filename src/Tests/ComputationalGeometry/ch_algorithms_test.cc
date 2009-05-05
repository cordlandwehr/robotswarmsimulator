#include <boost/test/unit_test.hpp>

#include "../ComputationalGeometry/ch_algorithms.h"

using namespace std;

BOOST_AUTO_TEST_CASE(point_contained_in_ch_polyhedron_test) {

	//create points
	std::vector<Vector3d> points;
	Vector3d tmp_point;
	Vector3d check_point;

	tmp_point(0)=0;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=1;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=0;
	tmp_point(1)=1;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=0;
	tmp_point(1)=0;
	tmp_point(2)=1;
	points.push_back(tmp_point);

	check_point(0)=1;
	check_point(1)=0;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

	check_point(0)=5;
	check_point(1)=5;
	check_point(2)=5;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), false);

	Vector3d cog = CHAlgorithms::compute_cog_of_ch_of_points(points);
	check_point(0)=cog(0);
	check_point(1)=cog(1);
	check_point(2)=cog(2);
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);
}

/*
BOOST_AUTO_TEST_CASE(point_contained_in_ch_segment_test) {

	//create points in line
	std::vector<Vector3d> points;
	Vector3d tmp_point;
	Vector3d check_point;

	tmp_point(0)=0;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=1;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=2;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=3;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	check_point(0)=0;
	check_point(1)=0;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

	check_point(0)=0.5;
	check_point(1)=0;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

	check_point(0)=0;
	check_point(1)=2;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), false);

	check_point(0)=1;
	check_point(1)=1;
	check_point(2)=1;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), false);
}
*/

BOOST_AUTO_TEST_CASE(point_contained_in_ch_point_test) {

	//create points
	std::vector<Vector3d> points;
	Vector3d tmp_point;
	Vector3d check_point;

	tmp_point(0)=0;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	check_point(0)=0;
	check_point(1)=0;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

	check_point(0)=1;
	check_point(1)=0;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), false);

}
