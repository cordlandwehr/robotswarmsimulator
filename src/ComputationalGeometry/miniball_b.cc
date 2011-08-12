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

#include "miniball_b.h"

// Miniball_b
// ----------

template <int d>
const double* Miniball_b<d>::center () const
{
  return current_c;
}

template <int d>
double Miniball_b<d>::squared_radius() const
{
  return current_sqr_r;
}

template <int d>
int Miniball_b<d>::size() const
{
  return m;
}

template <int d>
int Miniball_b<d>::support_size() const
{
  return s;
}

template <int d>
double Miniball_b<d>::excess (const Point<d>& p) const
{
  double e = -current_sqr_r;
  for (int k=0; k<d; ++k)
    e += mb_sqr(p[k]-current_c[k]);
  return e;
}



template <int d>
void Miniball_b<d>::reset ()
{
  m = s = 0;
  // we misuse c[0] for the center of the empty sphere
  for (int j=0; j<d; ++j)
    c[0][j]=0;
  current_c = c[0];
  current_sqr_r = -1;
}


template <int d>
void Miniball_b<d>::pop ()
{
  --m;
}


template <int d>
bool Miniball_b<d>::push (const Point<d>& p)
{
  int i, j;
  double eps = 1e-32;
  if (m==0) {
    for (i=0; i<d; ++i)
      q0[i] = p[i];
    for (i=0; i<d; ++i)
      c[0][i] = q0[i];
    sqr_r[0] = 0;
  } else {
    // set v_m to Q_m
    for (i=0; i<d; ++i)
      v[m][i] = p[i]-q0[i];

    // compute the a_{m,i}, i< m
    for (i=1; i<m; ++i) {
      a[m][i] = 0;
      for (j=0; j<d; ++j)
	a[m][i] += v[i][j] * v[m][j];
      a[m][i]*=(2/z[i]);
    }

    // update v_m to Q_m-\bar{Q}_m
    for (i=1; i<m; ++i) {
      for (j=0; j<d; ++j)
	v[m][j] -= a[m][i]*v[i][j];
    }

    // compute z_m
    z[m]=0;
    for (j=0; j<d; ++j)
      z[m] += mb_sqr(v[m][j]);
    z[m]*=2;

    // reject push if z_m too small
    if (z[m]<eps*current_sqr_r) {
      return false;
    }

    // update c, sqr_r
    double e = -sqr_r[m-1];
    for (i=0; i<d; ++i)
      e += mb_sqr(p[i]-c[m-1][i]);
    f[m]=e/z[m];

    for (i=0; i<d; ++i)
      c[m][i] = c[m-1][i]+f[m]*v[m][i];
    sqr_r[m] = sqr_r[m-1] + e*f[m]/2;
  }
  current_c = c[m];
  current_sqr_r = sqr_r[m];
  s = ++m;
  return true;
}

template <int d>
double Miniball_b<d>::slack () const
{
  double l[d+1], min_l=0;
  l[0] = 1;
  for (int i=s-1; i>0; --i) {
    l[i] = f[i];
    for (int k=s-1; k>i; --k)
      l[i]-=a[k][i]*l[k];
    if (l[i] < min_l) min_l = l[i];
    l[0] -= l[i];
  }
  if (l[0] < min_l) min_l = l[0];
  return ( (min_l < 0) ? -min_l : 0);
}
