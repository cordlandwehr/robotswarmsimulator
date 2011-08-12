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

#ifndef MINIBALL_B_H_
#define MINIBALL_B_H_

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


#include "miniball_point.h"

// Miniball_b
// ----------

template <int d>
class Miniball_b {
private:
  // data members
  int                 m, s;   // size and number of support points
  double              q0[d];

  double              z[d+1];
  double              f[d+1];
  double              v[d+1][d];
  double              a[d+1][d];

  double              c[d+1][d];
  double              sqr_r[d+1];

  double*             current_c;      // refers to some c[j]
  double              current_sqr_r;

public:
  Miniball_b() {reset();}

  // access
  const double*       center() const;
  double              squared_radius() const;
  int                 size() const;
  int                 support_size() const;
  double              excess (const Point<d>& p) const;

  // modification
  void                reset(); // generates empty sphere with m=s=0
  bool                push (const Point<d>& p);
  void                pop ();

  // checking
  double              slack() const;
};

#endif /* MINIBALL_B_H_ */
