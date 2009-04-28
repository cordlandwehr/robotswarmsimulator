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

#ifdef WIN32 // should work for MinGW, not tested for Visual Studio
#include <blaswrap.h>
#include <f2c.h>

/* ---------- Declarations of the used lapack methods (see sources of your lapack implementation) ---------- */
int dsyev_(char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work,
           integer *lwork, integer *info);
int dgesvd_(char *, char *, integer *, integer *, doublereal *, integer *, doublereal *, doublereal *, integer *,
            doublereal *, integer *, doublereal *, integer *, integer *);
#endif

// Under Mac OS X, the Accelerate framework is used (it includes an implementation of CLAPACK).
// We provide some common typedefs that are used in most versions of f2c.h.
#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
typedef __CLPK_doublereal doublereal;
typedef __CLPK_integer    integer;
#endif


#endif /* USE_LAPACK */

#endif /* LAPACK_HELPER_H */
