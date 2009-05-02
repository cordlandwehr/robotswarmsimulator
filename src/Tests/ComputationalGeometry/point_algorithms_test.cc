
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
