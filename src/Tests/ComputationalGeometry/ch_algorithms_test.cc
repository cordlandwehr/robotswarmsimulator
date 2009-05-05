#include <boost/test/unit_test.hpp>

#include "../ComputationalGeometry/ch_algorithms.h"

using namespace std;

BOOST_AUTO_TEST_CASE(ch_test_1) {

	//create some points
	std::vector<Vector3d> points;
	Vector3d tmp_point;

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

	//compute convex hull of created points
	CGAL::Object ch;
	ch = CHAlgorithms::compute_convex_hull_3d(points);

	//output vertices of computed convex hull
	//CHAlgorithms::print_vertices_of_ch(ch);

	//compute cog of computed convex hull
	Vector3d cog = CHAlgorithms::compute_cog_of_polyhedron(ch);

	//cout << "COG: " << cog << endl;

	//check if point is contained in convex hull of the point created above
	Vector3d check_point;

	check_point(0)=1;
	check_point(1)=0;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

	check_point(0)=5;
	check_point(1)=5;
	check_point(2)=5;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), false);

	check_point(0)=cog(0);
	check_point(1)=cog(1);
	check_point(2)=cog(2);
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

}
