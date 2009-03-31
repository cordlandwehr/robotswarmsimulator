#ifndef MINIBALL_H_
#define MINIBALL_H_

#include "miniball_point.h"
#include "miniball_b.h"

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

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include "../Utilities/vector_arithmetics.h"

// Functions
// =========
inline double mb_sqr (double r) {return r*r;}

// Miniball
// --------

template <int d>
class Miniball {
public:
  // types
  typedef typename std::list<Point<d> >::iterator         It;
  typedef typename std::list<Point<d> >::const_iterator   Cit;

private:
  // data members
  std::list<Point<d> > L;            // internal point set
  Miniball_b<d>        B;            // the current ball
  It                   support_end;  // past-the-end iterator of support set

  // private methods
  void        mtf_mb (It k);
  void        pivot_mb (It k);
  void        move_to_front (It j);
  double      max_excess (It t, It i, It& pivot) const;

public:
  // creates an empty ball
  Miniball() {}

  // copies p to the internal point set
  void        check_in (const Point<d>& p);

  // copies all vectors in vec as points to the internal point set
  void        check_in (const std::vector<Vector3d>& vec);

  // builds the smallest enclosing ball of the internal point set
  void        build ();

  // returns center of the ball (undefined if ball is empty)
  Point<d>    center() const;

  // returns squared_radius of the ball (-1 if ball is empty)
  double      squared_radius () const;

  // returns squared_radius of the ball (-1 if ball is empty)
  double      radius () const;

  // returns size of internal point set
  int         nr_points () const;

  // returns begin- and past-the-end iterators for internal point set
  Cit         points_begin () const;
  Cit         points_end () const;

  // returns size of support point set; this set has the following properties:
  // - there are at most d+1 support points,
  // - all support points are on the boundary of the computed ball, and
  // - the smallest enclosing ball of the support point set equals the
  //   smallest enclosing ball of the internal point set
  int         nr_support_points () const;

  // returns begin- and past-the-end iterators for internal point set
  Cit         support_points_begin () const;
  Cit         support_points_end () const;

  // assesses the quality of the computed ball. The return value is the
  // maximum squared distance of any support point or point outside the
  // ball to the boundary of the ball, divided by the squared radius of
  // the ball. If everything went fine, this will be less than e-15 and
  // says that the computed ball approximately contains all the internal
  // points and has all the support points on the boundary.
  //
  // The slack parameter that is set by the method says something about
  // whether the computed ball is really the *smallest* enclosing ball
  // of the support points; if everything went fine, this value will be 0;
  // a positive value may indicate that the ball is not smallest possible,
  // with the deviation from optimality growing with the slack
  double      accuracy (double& slack) const;

  // returns true if the accuracy is below the given tolerance and the
  // slack is 0
  bool        is_valid (double tolerance = 1e-15) const;
};


#endif /* MINIBALL_H_ */
