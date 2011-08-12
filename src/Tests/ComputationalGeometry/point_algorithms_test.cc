/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

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
#include <boost/smart_ptr.hpp>

#include "../../ComputationalGeometry/point_algorithms.h"

#include "../../Utilities/vector_arithmetics.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(MEDline_test) {
	std::vector<Vector3d> positions;

  	Vector3d a;
	a(0) = 1.0;
	a(1) = 4.0;
	a(2) = 10.0;

	Vector3d b;
	b(0) = 2.2;
	b(1) = 3.0;
	b(2) = 0.0;

	Vector3d c;
	c(0) = 2.0;
	c(1) = 9.0;
	c(2) = 2.0;

	Vector3d d;
	d(0) = 7.0;
	d(1) = 6.0;
	d(2) = 3.0;

	positions.push_back(a);
	positions.push_back(b);
	positions.push_back(c);
	positions.push_back(d);

	Vector3d result = PointAlgorithms::compute_MED(positions);

	BOOST_CHECK_CLOSE(result(0), 2.2, 0.0001);
	BOOST_CHECK_CLOSE(result(1), 6.0, 0.0001);
	BOOST_CHECK_CLOSE(result(2), 3.0, 0.0001);
}

BOOST_AUTO_TEST_CASE(maxline_test) {
	std::vector<Vector3d> positions;

  	Vector3d a;
	a(0) = 1.0;
	a(1) = 4.0;
	a(2) = 10.0;

	Vector3d b;
	b(0) = 2.2;
	b(1) = 3.0;
	b(2) = 0.0;

	Vector3d c;
	c(0) = 2.0;
	c(1) = 9.0;
	c(2) = 2.0;

	Vector3d d;
	d(0) = 7.0;
	d(1) = 6.0;
	d(2) = 3.0;

	positions.push_back(a);
	positions.push_back(b);
	positions.push_back(c);
	positions.push_back(d);

	Vector3d result = PointAlgorithms::compute_MaxLine(kXCoord, positions);

	BOOST_CHECK_CLOSE(result(0), 7.0, 0.0001);
	BOOST_CHECK_CLOSE(result(1), 6.0, 0.0001);
	BOOST_CHECK_CLOSE(result(2), 3.0, 0.0001);
}

BOOST_AUTO_TEST_CASE(midfar_test) {
	std::vector<Vector3d> positions;

  	Vector3d a;
	a(0) = 1.0;
	a(1) = 4.0;
	a(2) = 10.0;

	Vector3d b;
	b(0) = 2.2;
	b(1) = 3.0;
	b(2) = 0.0;

	Vector3d c;
	c(0) = 2.0;
	c(1) = 9.0;
	c(2) = 2.0;

	Vector3d d;
	d(0) = 7.0;
	d(1) = 6.0;
	d(2) = 3.0;

	positions.push_back(a);
	positions.push_back(b);
	positions.push_back(c);
	positions.push_back(d);

	Vector3d result = PointAlgorithms::compute_MidFar(a, positions);

	BOOST_CHECK_CLOSE(result(0), 1.6, 0.0001);
	BOOST_CHECK_CLOSE(result(1), 3.5, 0.0001);
	BOOST_CHECK_CLOSE(result(2), 5.0, 0.0001);
}

BOOST_AUTO_TEST_CASE(cbox_test) {

	std::vector<Vector3d> positions;


  	Vector3d a;
	a(0) = 1.0;
	a(1) = 4.0;
	a(2) = 10.0;

	Vector3d b;
	b(0) = 2.2;
	b(1) = 3.0;
	b(2) = 0.0;

	Vector3d c;
	c(0) = 2.0;
	c(1) = 9.0;
	c(2) = 2.0;

	Vector3d d;
	d(0) = 7.0;
	d(1) = 6.0;
	d(2) = 3.0;

	positions.push_back(a);
	positions.push_back(b);
	positions.push_back(c);
	positions.push_back(d);

	Vector3d result = PointAlgorithms::compute_CMinBox(positions);

	BOOST_CHECK_CLOSE(result(0), 4.0, 0.0001);
	BOOST_CHECK_CLOSE(result(1), 6.0, 0.0001);
	BOOST_CHECK_CLOSE(result(2), 5.0, 0.0001);

}
