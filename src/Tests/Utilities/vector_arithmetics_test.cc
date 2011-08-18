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

/*
 *  vector_arithmetics_test.cpp
 *
 *  Created on: Feb 6, 2009
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "../../Utilities/vector_arithmetics.h"


/*
 * Check computation of determinant for some 3x3 matrices.
 */
BOOST_AUTO_TEST_CASE(determinant_test) {
	Matrix3d zero_matrix = boost::numeric::ublas::zero_matrix<double>(3,3);
	Matrix3d unit_matrix = boost::numeric::ublas::identity_matrix<double>(3);
	
	Matrix3d matrix1 = Matrix3d(3,3);
	matrix1(0,0) =  2; matrix1(0,1) =  3.0; matrix1(0,2) = - 7;
	matrix1(1,0) =  0; matrix1(1,1) = -0.1; matrix1(1,2) =  49;
	matrix1(2,0) = -7; matrix1(2,1) =  1.5; matrix1(2,2) = - 1;
	
	BOOST_CHECK_CLOSE(det_matrix3d(zero_matrix),     0.0, 0.1);
	BOOST_CHECK_CLOSE(det_matrix3d(unit_matrix),     1.0, 0.1);
	BOOST_CHECK_CLOSE(det_matrix3d(matrix1),     -1170.9, 0.1);
}


/*
 * Check computation of inverse for some 3x3 matrices.
 */
BOOST_AUTO_TEST_CASE(matrix_inversion_test) {
	Matrix3d zero_matrix = boost::numeric::ublas::zero_matrix<double>(3,3);
	Matrix3d unit_matrix = boost::numeric::ublas::identity_matrix<double>(3);
	
	Matrix3d matrix1 = Matrix3d(3,3);
	matrix1(0,0) =  2; matrix1(0,1) =  3.0; matrix1(0,2) = - 7;
	matrix1(1,0) =  0; matrix1(1,1) = -0.1; matrix1(1,2) =  49;
	matrix1(2,0) = -7; matrix1(2,1) =  1.5; matrix1(2,2) = - 1;
	
	// inverse of matrix 1
	Matrix3d matrix1_actual_inverse = Matrix3d(3,3);
	matrix1_actual_inverse(0,0) = 0.0626868221000; matrix1_actual_inverse(0,1) = 0.006405329234; matrix1_actual_inverse(0,2) = -0.1249466223000;
	matrix1_actual_inverse(1,0) = 0.2929370570000; matrix1_actual_inverse(1,1) = 0.043556238790; matrix1_actual_inverse(1,2) =  0.0836963019900;
	matrix1_actual_inverse(2,0) = 0.0005978307285; matrix1_actual_inverse(2,1) = 0.020497053550; matrix1_actual_inverse(2,2) =  0.0001708087796;
	
	Matrix3d unit_matrix_inverse = invert_matrix3d(unit_matrix);
	Matrix3d matrix1_inverse = invert_matrix3d(matrix1);
	
	BOOST_CHECK_THROW(invert_matrix3d(zero_matrix), std::invalid_argument);

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			BOOST_CHECK_CLOSE(unit_matrix_inverse(i,j), unit_matrix(i,j), 0.1);
	
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			BOOST_CHECK_CLOSE(matrix1_inverse(i,j), matrix1_actual_inverse(i,j), 0.1);
}
