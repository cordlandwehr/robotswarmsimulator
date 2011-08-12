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

#ifndef MINIBALL_POINT_H_
#define MINIBALL_POINT_H_

//    Copright (C) 1999-2006, Bernd Gaertner
//    $Revision: 1.3 $
//    $Date: 2006/11/16 08:01:52 $
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA,
//    or download the License terms from prep.ai.mit.edu/pub/gnu/COPYING-2.0.
//
//    Contact:
//    --------
//    Bernd Gaertner
//    Institute of Theoretical Computer Science
//    ETH Zuerich
//    CAB G32.2
//    CH-8092 Zuerich, Switzerland
//    http://www.inf.ethz.ch/personal/gaertner

#include <iostream>
#include <cmath>
#include "../Utilities/vector_arithmetics.h"

// Point (inline)
// -------------
template <int d>
class Point {
private:
  double coord [d];

public:
  // default
  Point()
  {}

  // copy from Point
  Point (const Point& p)
  {
    for (int i=0; i<d; ++i)
      coord[i] = p.coord[i];
  }

  // copy from double*
  Point (const double* p)
  {
    for (int i=0; i<d; ++i)
      coord[i] = p[i];
  }

  // assignment
  Point& operator = (const Point& p)
  {
    if (this != &p)
      for (int i=0; i<d; ++i)
	coord[i] = p.coord[i];
    return *this;
  }

  // coordinate access
  double& operator [] (int i)
  {
    return coord[i];
  }
  const double& operator [] (int i) const
  {
    return coord[i];
  }
  const double* begin() const
  {
    return coord;
  }
  const double* end() const
  {
    return coord+d;
  }
  const double dist(const Vector3d& v) const
  {
	if (d < 3)
		std::cout << "must not call Point<d>.dist(...) for d<3" << std::endl;

	double dx = v[0]-coord[0];
	double dy = v[1]-coord[1];
	double dz = v[2]-coord[2];
	return sqrt(dx*dx+dy*dy+dz*dz);
  }
  const void set_other(Vector3d& v) const
  {
	if (d < 3)
		std::cout << "must not call Point<d>.dist(...) for d<3" << std::endl;

	v[0]=coord[0];
	v[1]=coord[1];
	v[2]=coord[2];
  }
};

#endif /* MINIBALL_POINT_H_ */
