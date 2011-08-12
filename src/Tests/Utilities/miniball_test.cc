/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>

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

#include "../../ComputationalGeometry/miniball.h"
#include "../../ComputationalGeometry/miniball.cc"
#include "../../ComputationalGeometry/miniball_b.cc"

#include "../../Utilities/vector_arithmetics.h"

// Point
// -----

// Output
template <int d>
std::ostream& operator << (std::ostream& os, const Point<d>& p)
{
  os << "(";
  for (int i=0; i<d-1; ++i)
    os << p[i] << ", ";
  os << p[d-1] << ")";
  return os;
}

BOOST_AUTO_TEST_CASE(miniball_test) {

	bool DEBUG = false;

	using std::cout;
	using std::endl;

	if (DEBUG)
		cout << "miniball_test" << endl;

	std::vector<Vector3d> statistik;
  	std::vector<double> stat_data;

  	Vector3d a;
	a(0) = 0.0;
	a(1) = 0.0;
	a(2) = 0.0;

	Vector3d b;
	b(0) = 2.0;
	b(1) = 0.0;
	b(2) = 0.0;

	Vector3d c;
	c(0) = 0.0;
	c(1) = 2.0;
	c(2) = 0.0;

	Vector3d d;
	d(0) = 0.0;
	d(1) = 0.0;
	d(2) = 0.0;

	statistik.push_back(a);
	statistik.push_back(b);
	statistik.push_back(c);
	statistik.push_back(d);

	if (DEBUG)
		for (unsigned int i=0; i<statistik.size(); i++) {
			cout << statistik[i] << endl;
		}

	Miniball<3>     mb;

  mb.check_in(statistik);

  // construct ball
  // --------------
  if (DEBUG)
	  std::cout << "Constructing miniball..."; //cout.flush();

  mb.build();

  if (DEBUG) {
	  std::cout << "done." << std::endl << std::endl;

	  // output center and squared radius
	  // --------------------------------
	  cout << "Center:         " << mb.center() << endl;
	  cout << "Squared radius: " << mb.squared_radius() << endl << endl;
	  cout << "Radius: " << mb.radius() << endl << endl;
  }

  BOOST_CHECK_EQUAL(mb.center()[0], 1);
  BOOST_CHECK_EQUAL(mb.center()[1], 1);
  BOOST_CHECK_EQUAL(mb.center()[2], 0);
  BOOST_CHECK_CLOSE(mb.radius(), 1.41421, 0.1);
}
