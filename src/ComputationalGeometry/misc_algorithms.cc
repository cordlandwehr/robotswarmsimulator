/*
 * Geometry.cpp
 *
 *  Created on: Mar 18, 2009
 *      Author: phoenixx
 */

#include "misc_algorithms.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>

#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#include <ComputationalGeometry/point_algorithms.h>

#include <Utilities/console_output.h>
#include <Utilities/lapack.h>


const double MiscAlgorithms::kEpsilon = 0.000000001;


bool MiscAlgorithms::compare_vectors_by_axis (const Vector3d& vec_a, const Vector3d& vec_b, int axis) {
	return vec_a(axis) <= vec_b(axis);
}

bool MiscAlgorithms::compare_vectors_by_axis_x (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axis (vec_a, vec_b, 0);
}

bool MiscAlgorithms::compare_vectors_by_axis_y (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axis (vec_a, vec_b, 0);
}

bool MiscAlgorithms::compare_vectors_by_axis_z (const Vector3d& vec_a, const Vector3d& vec_b) {
	return compare_vectors_by_axis (vec_a, vec_b, 0);
}

bool MiscAlgorithms::are_points_on_plane(std::vector<Vector3d> point_list) {
	// if less than for points return true:
	if (point_list.size() < 4)
		return true;

	// else compute Hesse-Form and test distances
	// this is the normal vector by cross product
	Vector3d normal;
	normal(0) = point_list.at(0)(1)*point_list.at(1)(2)-point_list.at(0)(2)*point_list.at(1)(1);
	normal(1) =	point_list.at(0)(2)*point_list.at(1)(0)-point_list.at(0)(0)*point_list.at(1)(2);
	normal(2) = point_list.at(0)(0)*point_list.at(1)(1)-point_list.at(0)(1)*point_list.at(1)(0);

	// distance: layer to origin
	double distance =  normal(0)*point_list.at(2)(0) + normal(1)*point_list.at(2)(1) + normal(2)*point_list.at(2)(2);

	// checks if all points are on layer
	for (unsigned i=3; i < point_list.size(); i++) {
		if (std::fabs( normal(0)*point_list.at(i)(0) + normal(1)*point_list.at(i)(1) + normal(2)*point_list.at(i)(2) - distance)
				> MiscAlgorithms::kEpsilon)
			return false;
	}

	// thus all points are at the layer
	return true;
}

bool MiscAlgorithms::is_point_in_smallest_bbox(std::vector<Vector3d> point_list, const Vector3d& testpoint) {
	std::vector<Vector3d>::iterator iter;

	std::sort (point_list.begin(), point_list.end(), MiscAlgorithms::compare_vectors_by_axis_x);
	if (testpoint(0) < (*point_list.begin())(0) || testpoint(0) > (*point_list.end())(0))
		return false;

	std::sort (point_list.begin(), point_list.end(), MiscAlgorithms::compare_vectors_by_axis_y);
	if (testpoint(1) < (*point_list.begin())(1) || testpoint(1) > (*point_list.end())(1))
		return false;

	std::sort (point_list.begin(), point_list.end(), MiscAlgorithms::compare_vectors_by_axis_z);
	if (testpoint(2) < (*point_list.begin())(2) || testpoint(2) > (*point_list.end())(2))
		return false;

	return true;
}

void MiscAlgorithms::sort_points_by_distance(std::vector<Vector3d>& point_list, int p) {
	sort (point_list.begin(), point_list.end(), (boost::bind(vector3d_get_length, _1, p) < boost::bind(vector3d_get_length, _2, p)));
}

void MiscAlgorithms::sort_pointslist_by_distance(std::vector< std::pair<Vector3d,std::size_t> >& point_list, int p) {
	sort (point_list.begin(), point_list.end(), (boost::bind(vector3d_get_length, boost::bind(&std::pair<Vector3d,std::size_t>::first,_1), p) < boost::bind(vector3d_get_length, boost::bind(&std::pair<Vector3d,std::size_t>::first,_2), p)));
}


#ifndef USE_LAPACK
boost::tuple<Vector3d,Vector3d> MiscAlgorithms::calculate_shim_plane(const std::vector<Vector3d>& pos) {
	ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error)
		<< "LAPACK not supported on your system (in 'MiscAlgorithms::calculate_shim_plane')";
	exit(1);
}
#else
boost::tuple<Vector3d,Vector3d> MiscAlgorithms::calculate_shim_plane(const std::vector<Vector3d>& pos) {
	// compute the cog (as a point lying on the to be computed shim plane)
	const Vector3d cog = PointAlgorithms::compute_COG(pos);

	// TODO(Peter@Kamil) comment (why have the points to be sorted?)
	std::vector<Vector3d> positions(pos);
	sort(positions.begin(), positions.end(), (boost::bind(vector3d_distance, _1, cog) < boost::bind(vector3d_distance, _2, cog)));

	// setup matrix (this is a symmetrical matrix!)
	// TODO(Peter@Kamil) Please have a look at the setup of this matrix. The setup should be equivalent to the code you
	//     mailed me, but it looks somewhat strange. For once, you used a symmetric matrix type, but nevertheless set
	//     every matrix element (instead of only one triangular part). Furthermore, you should revisit the innermost for
	//     loop (actually, only it's last iteration has an effect on the matrix).
	const unsigned int dim = 3;
	doublereal matrix[dim*dim];
	for (unsigned int i=0; i<dim; i++) {
		for (unsigned int j=0; j<dim; j++) {
			// index 'j*dim+i' addresses the element in the i-th row and j-th column (we have to use column major
			// ordering due to compatibility with fortran)
			matrix[j*dim+i] = 0.0;
			for (unsigned int l=0; l<positions.size(); l++) {
				matrix[j*dim+i] = (positions.at(l)(i) - cog(i)) * (positions.at(l)(j) - cog(j));
			}
		}
	}


	/* ------------------------- BEGIN: solve the eigenvalue problem using LAPACK ------------------------- */
	// input variables for LAPACK call
	char jobz = 'V'; // compute eigenvalues and eigenvectors
	char uplo = 'L'; // lower triangle of matrix is stored
	integer N = dim; // order of the matrix (i.e. dimension)
	integer lda = dim; // leading dimension of matrix array (in our case the matrix dimension)
	integer lwork = 3*N; // length of the work array; note: this is not optimal, see documentation of 'dsyev'

	// output variables for LAPACK call
	integer info; // error value (0 = successful)
	doublereal eigvals[dim]; // storage for the eigenvalues (will be stored in ascending order)
	doublereal work[9]; // memory for the 'dsyev' method to work in

	// input/output variables for LAPACK call
	// the matrix parameter is used as input (the matrix whose eigenvals to compute) and output (eigenvectors)
	// note that the eigenvectors will form an ortonormal system (especially: they are already normalized!)

	// compute the eigenvalues/-vectors by a call to the LAPACK method 'DSYEV' (Double, SYmmetric matrix, EigenValues)
	dsyev_(&jobz, &uplo, &N, matrix, &lda, eigvals, work, &lwork, &info);

	// error checking
	if (info != 0) {
		ConsoleOutput::log(ConsoleOutput::ComputationalGeometry, ConsoleOutput::error)
			<< "error in eigenvalue computation (in 'MiscAlgorithms::calculate_shim_plane')";
		exit(1);
	}
	/* ------------------------- END: solve the eigenvalue problem using LAPACK ------------------------- */

	// The matrix now contains the resulting eigenvectors (first column corresponds to first eigenvector and so on...).
	// We take the eigenvector corresponding to the smallest eigenvalue (the first one, see LAPACK documentation).
	Vector3d normal;
	normal(0) = matrix[0*dim+0];
	normal(1) = matrix[0*dim+1];
	normal(2) = matrix[0*dim+2];
	return boost::make_tuple(cog, normal);
}
#endif

// Cuts the length of the given targetposition tp in-place such that the
// visibility to the given positions of neighbours (already with distance <= view radius v)
// is kept. Therefore the trivial (direction-independant) algorithm is applied.
// Remark that it is no problem to have the current position of the robot (0,0,0)
// itself contained in positions.
void MiscAlgorithms::cut_trivial(Vector3d & tp, std::vector<Vector3d> & positions, double v) {
	// calculate maximal distance to any robot
	double maxDist = 0.0;
	for (int i=0; i<positions.size(); i++) {
		double dist = vector3d_distance(positions[0], positions[i]);
		if (dist > maxDist)
			maxDist = dist;
	}

	// now let maxDist the maximum distance of tp to self in (0,0,0)
	maxDist = (v-maxDist)/2.0;

	// trim tp to length maxDist if neccessary
	vector3d_set_maxlength(tp, maxDist);
}

void MiscAlgorithms::cut_maxmove(Vector3d & tp, std::vector<Vector3d> & positions, double v) {
	bool DEBUG = false;

	double maxDistStart = 0.0;
	for (int i=0; i<positions.size(); i++) {
		double curDist = vector3d_distance(tp, positions[i], 2);
		if(curDist > maxDistStart)
			maxDistStart = curDist;
	}

	double min_t = 1.0;

	for (int i=0; i<positions.size(); i++) {
		Vector3d q = positions[i];
		if(q[0]==0 && q[1]==0 && q[2]==0)
			continue;

		// calculate distance of target tp to the middlepos of me and other q/2
		if (vector3d_distance(tp, q/2.0) > v/2.0) {
			// calculate some magic a,b,c (from maths behind)
			double a, b, c;
			a = tp[0]*tp[0] + tp[1]*tp[1] + tp[2]*tp[2];
			b = -q[0]*tp[0]-q[1]*tp[1]-q[2]*tp[2];
			c = (q[0]*q[0] + q[1]*q[1] + q[2]*q[2] - v*v)/4.0;

			// do some integrity check
			if (b*b-4*a*c < 0) {
				std::cout << "ERROR: 0 > b*b-4*a*c = " << b*b-4*a*c << std::endl;
				std::cout << " for tp=" << tp << " q= " << q << " (self at=" << positions[0] << ")" << std::endl;
			}

			// calculate both solutions of a * t^2 + b*t + c = 0
			double t1 = (-b - sqrt(b*b-4*a*c))/(2*a);
			double t2 = (-b + sqrt(b*b-4*a*c))/(2*a);

			// we might assert: Either t1=t2=0
			// or there exists exactly one tj with tj in ]0,1[ and the other is < 0
			// so we use the bigger to shorten our tp nearer to self position along direction
			double t = std::max(t1, t2);

			// handle some rounding errors
			if (t > 0.0001 && t < 0.0001)
				t = 0.0;

			// do some integrity check
			if (t < 0) {
				std::cout << "ERROR: t1=" << t1 << ", t2=" << t2;
				std::cout << " for tp=" << tp << " q= " << q << " (self at=" << positions[0] << ")" << std::endl;
			}

			if (t < min_t)
				min_t = t;
		}
	}

	// stretch tp by given factor
	tp = min_t * tp;

	if (DEBUG && min_t < 1.0)
		std::cout << " new tp=" << tp << " (by t=" << min_t << ")" << std::endl;

	// check new maxDist
	double maxDistEnd = 0.0;
	for (int i=0; i<positions.size(); i++) {
		double curDist = vector3d_distance(tp, positions[i], 2);
		if(curDist > maxDistEnd)
			maxDistEnd = curDist;
	}

	// simply cut to avoid latter rounding errors
	// due to robots to near at sightradius
	if (maxDistEnd > v-0.001) {
		tp[0]=tp[1]=tp[2]=0;
	}
}
