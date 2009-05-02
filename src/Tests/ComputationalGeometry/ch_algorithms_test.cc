#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <CGAL/Homogeneous.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/copy_n.h>
#include <CGAL/Convex_hull_traits_3.h>
#include <CGAL/convex_hull_3.h>
#include <vector>
#include <CGAL/MP_Float.h>

#include "../ComputationalGeometry/ch_algorithms.h"

using namespace std;

BOOST_AUTO_TEST_CASE(ch_test_1) {

	//cout << "####################### BEGIN: CH-TEST 1 ####################################" << endl;

	//generate 250 points randomly on a sphere of radius 100.0
	CGAL::Random_points_in_sphere_3<Point_3, PointCreator> gen(100.0);

	//copy generated points to a vector
	std::vector<Point_3> points_3;
	CGAL::copy_n(gen, 250, std::back_inserter(points_3));

	//create Vector3d out of Point_3
	std::vector<Vector3d> points;
	Point_3 cur_point;
	Vector3d p;
	for(int i=0; i<points_3.size(); i++) {
		p(0) = points_3[i].x();
		p(1) = points_3[i].y();
		p(2) = points_3[i].z();
		points.push_back(p);
	}

	CGAL::Object ch_object;
	ch_object = CHAlgorithms::compute_convex_hull_3d(points);

	// determine what kind of object it is
	if (CGAL::object_cast<Segment_3>(&ch_object) )
		cout << "convex hull is a segment " << std::endl;
	else if (CGAL::object_cast<Polyhedron_3>(&ch_object) )
		cout << "convex hull is a polyhedron " << std::endl;
	else
		cout << "convex hull error!" << std::endl;

	Vector3d cog = CHAlgorithms::compute_cog_of_polyhedron(ch_object);

	//cout << "####################### END: CH-TEST 1 ####################################" << endl;
}
