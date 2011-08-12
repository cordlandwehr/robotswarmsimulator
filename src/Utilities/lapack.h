/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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

/**
 * \file lapack.h
 * \author Peter Kling
 * \date 26.04.09
 *
 * Include this header if you want to use a method from LAPACK. This file wraps the neccessary includes and so on for
 * the different operating systems. It is assumed that CLAPACK is used.
 */

#ifndef LAPACK_HELPER_H
#define LAPACK_HELPER_H

#ifndef USE_LAPACK // TODO: remove this protector as soon as (nearly) everybody has lapack installed
// do not try to include anything if lapack not supported on this system
#else


#ifdef __APPLE__
// Under Mac OS X, the Accelerate framework is used (it includes an implementation of CLAPACK).
// We provide some common typedefs that are used in most versions of f2c.h.

#include <Accelerate/Accelerate.h>
typedef __CLPK_doublereal doublereal;
typedef __CLPK_integer    integer;

#else

extern "C" {
	// extra headers/declarations for Linux and Windows
	#include <Utilities/blaswrap.h>
	#include <Utilities/f2c.h>

	/* ---------- Declarations of the used lapack methods (see sources of your lapack implementation) ---------- */
	int dsyev_(char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work,
	           integer *lwork, integer *info);
	int dgesvd_(char *, char *, integer *, integer *, doublereal *, integer *, doublereal *, doublereal *, integer *,
	            doublereal *, integer *, doublereal *, integer *, integer *);
}

#endif /* __APPLE__ */


#endif /* USE_LAPACK */

#endif /* LAPACK_HELPER_H */
