#include "miniball.h"

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

// Class Implementations
// =====================

// Miniball
// --------

template <int d>
void Miniball<d>::check_in (const Point<d>& p)
{
  L.push_back(p);
}

template <int d>
void Miniball<d>::check_in (const Vector3d& p) {
	if (d != 3) {
		std::cerr << "must not call Miniball<d>::check_in (const Vector3d& p) with d <> 3" << std::endl;
		return;
	}

	Point<3> ppoint;
	ppoint[0] = p[0];
	ppoint[1] = p[1];
	ppoint[2] = p[2];
	check_in(ppoint);
}

template <int d>
void Miniball<d>::check_in (const std::vector<Vector3d>& vec)
{
	if (d != 3) {
		std::cerr << "must not call Miniball<d>::check_in (const std::vector<Vector3d>& p) with d <> 3" << std::endl;
		return;
	}

	for (unsigned int i=0; i<vec.size(); i++) {
		Point<3>* p = new Point<3>(&vec[i][0]);
		L.push_back(*p);
	}
}

template <int d>
void Miniball<d>::build ()
{
  B.reset();
  support_end = L.begin();
  pivot_mb (L.end());
}


template <int d>
void Miniball<d>::mtf_mb (It i)
{
  support_end = L.begin();
  if ((B.size())==d+1) return;
  for (It k=L.begin(); k!=i;) {
    It j=k++;
    if (B.excess(*j) > 0) {
      if (B.push(*j)) {
	mtf_mb (j);
	B.pop();
	move_to_front(j);
      }
    }
  }
}

template <int d>
void Miniball<d>::move_to_front (It j)
{
  if (support_end == j)
    support_end++;
  L.splice (L.begin(), L, j);
}


template <int d>
void Miniball<d>::pivot_mb (It i)
{
  It t = ++L.begin();
  mtf_mb (t);
  double max_e, old_sqr_r = -1;
  do {
    It pivot;
    max_e = max_excess (t, i, pivot);
    if (max_e > 0) {
      t = support_end;
      if (t==pivot) ++t;
      old_sqr_r = B.squared_radius();
      B.push (*pivot);
      mtf_mb (support_end);
      B.pop();
      move_to_front (pivot);
    }
  } while ((max_e > 0) && (B.squared_radius() > old_sqr_r));
}


template <int d>
double Miniball<d>::max_excess (It t, It i, It& pivot) const
{
  const double *c = B.center(), sqr_r = B.squared_radius();
  double e, max_e = 0;
  for (It k=t; k!=i; ++k) {
    const double *p = (*k).begin();
    e = -sqr_r;
    for (int j=0; j<d; ++j)
      e += mb_sqr(p[j]-c[j]);
    if (e > max_e) {
      max_e = e;
      pivot = k;
    }
  }
  return max_e;
}



template <int d>
Point<d> Miniball<d>::center () const
{
  return Point<d>(B.center());
}

template <int d>
double Miniball<d>::squared_radius () const
{
  return B.squared_radius();
}

template <int d>
double Miniball<d>::radius () const
{
	double sr = squared_radius();
  	if (sr == -1)
  		return -1;
  	else
  		return sqrt(sr);
}


template <int d>
int Miniball<d>::nr_points () const
{
  return L.size();
}

template <int d>
typename Miniball<d>::Cit Miniball<d>::points_begin () const
{
  return L.begin();
}

template <int d>
typename Miniball<d>::Cit Miniball<d>::points_end () const
{
  return L.end();
}


template <int d>
int Miniball<d>::nr_support_points () const
{
  return B.support_size();
}

template <int d>
typename Miniball<d>::Cit Miniball<d>::support_points_begin () const
{
  return L.begin();
}

template <int d>
typename Miniball<d>::Cit Miniball<d>::support_points_end () const
{
  return support_end;
}



template <int d>
double Miniball<d>::accuracy (double& slack) const
{
  double e, max_e = 0;
  int n_supp=0;
  Cit i;
  for (i=L.begin(); i!=support_end; ++i,++n_supp)
    if ((e = std::abs (B.excess (*i))) > max_e)
      max_e = e;

  // you've found a non-numerical problem if the following ever fails
  assert (n_supp == nr_support_points());

  for (i=support_end; i!=L.end(); ++i)
    if ((e = B.excess (*i)) > max_e)
      max_e = e;

  slack = B.slack();
  return (max_e/squared_radius());
}


template <int d>
bool Miniball<d>::is_valid (double tolerance) const
{
  double slack;
  return ( (accuracy (slack) < tolerance) && (slack == 0) );
}

