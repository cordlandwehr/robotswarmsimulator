/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/test/unit_test.hpp>

#include "../ComputationalGeometry/ch_algorithms.h"

using namespace std;

BOOST_AUTO_TEST_CASE(point_contained_in_ch_polyhedron_test) {

	std::vector<Vector3d> points;
	Vector3d tmp_point;
	Vector3d check_point;

	//create points whose convex hull is a cube
	tmp_point(0)=0;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=1;
	tmp_point(1)=0;
	tmp_point(2)=0;
	points.push_back(tmp_point);

	tmp_point(0)=1;
	tmp_point(1)=1;
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

	tmp_point(0)=1;
	tmp_point(1)=0;
	tmp_point(2)=1;
	points.push_back(tmp_point);

	tmp_point(0)=1;
	tmp_point(1)=1;
	tmp_point(2)=1;
	points.push_back(tmp_point);

	tmp_point(0)=0;
	tmp_point(1)=1;
	tmp_point(2)=1;
	points.push_back(tmp_point);

	//point in the convex hull of the quad defined above
	check_point(0)=0.5;
	check_point(1)=0.5;
	check_point(2)=0.5;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

	//point outside the convex hull of the quad defined above
	check_point(0)=5;
	check_point(1)=5;
	check_point(2)=5;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), false);

	//point on the boundary of the convex hull
	check_point(0)=0;
	check_point(1)=0;
	check_point(2)=0;
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);


	Vector3d cog = CHAlgorithms::compute_cog_of_ch_of_points(points);
	check_point(0)=cog(0);
	check_point(1)=cog(1);
	check_point(2)=cog(2);
	BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(check_point, points), true);

}


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

BOOST_AUTO_TEST_CASE(rnd_poin_in_ch) {

	//create points
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

	tmp_point(0)=1;
	tmp_point(1)=1;
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

	tmp_point(0)=1;
	tmp_point(1)=0;
	tmp_point(2)=1;
	points.push_back(tmp_point);

	tmp_point(0)=1;
	tmp_point(1)=1;
	tmp_point(2)=1;
	points.push_back(tmp_point);

	tmp_point(0)=0;
	tmp_point(1)=1;
	tmp_point(2)=1;
	points.push_back(tmp_point);

	Vector3d rnd_point;
	for(int i=0; i<10; i++) {
		rnd_point = CHAlgorithms::random_point_in_ch(points, i);
		//cout << "("<<rnd_point(0)<<","<<rnd_point(1)<<","<<rnd_point(2)<<")" << endl;

		//check whether random point is contained in convex hull (should always return true)
		BOOST_CHECK_EQUAL(CHAlgorithms::point_contained_in_convex_hull_of_points(rnd_point, points), true);
	}
}
