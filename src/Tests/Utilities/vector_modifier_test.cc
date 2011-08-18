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

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Utilities/VectorModifiers/vector_difference_trimmer.h"
#include "../../Utilities/VectorModifiers/vector_trimmer.h"


/*
 * Test: Trim several vectors to zero length.
 */
BOOST_AUTO_TEST_CASE(vector_trimmer_zero_test) {
	using boost::numeric::ublas::zero_vector;
	using boost::numeric::ublas::unit_vector;

	Vector3d vector1 = zero_vector<double>(3);
	Vector3d vector2 = unit_vector<double>(3, 2);

	// length: 110.6788729
	Vector3d vector3;
	vector3(0) = -102.4;
	vector3(1) = -0.23;
	vector3(2) = 42.;

	// apply vector modifier
	double length = 0.;
	VectorTrimmer vector_modifier(length);
	vector_modifier.modify_vector(vector1, Vector3d());
	vector_modifier.modify_vector(vector2, Vector3d());
	vector_modifier.modify_vector(vector3, Vector3d());

	// all components must be near zero
	BOOST_CHECK_SMALL(vector1(0), 1E-10);
	BOOST_CHECK_SMALL(vector1(1), 1E-10);
	BOOST_CHECK_SMALL(vector1(2), 1E-10);
	BOOST_CHECK_SMALL(vector2(0), 1E-10);
	BOOST_CHECK_SMALL(vector2(1), 1E-10);
	BOOST_CHECK_SMALL(vector2(2), 1E-10);
	BOOST_CHECK_SMALL(vector3(0), 1E-10);
	BOOST_CHECK_SMALL(vector3(1), 1E-10);
	BOOST_CHECK_SMALL(vector3(2), 1E-10);
}


/*
 * Test: Trim several vectors to non-zero length.
 */
BOOST_AUTO_TEST_CASE(vector_trimmer_non_zero_test) {
	using boost::numeric::ublas::zero_vector;
	using boost::numeric::ublas::unit_vector;
	using boost::numeric::ublas::norm_2;
	using boost::numeric::ublas::outer_prod;

	Vector3d vector1 = zero_vector<double>(3);
	Vector3d vector2 = unit_vector<double>(3, 2);

	// length: 110.6788729
	Vector3d vector3;
	vector3(0) = -102.4;
	vector3(1) = -0.23;
	vector3(2) = 42.;

	// length: 5.458937626
	Vector3d vector4;
	vector4(0) = 3;
	vector4(1) = 4.4;
	vector4(2) = -1.2;

	// copies
	Vector3d vector1_original = vector1;
	Vector3d vector2_original = vector2;
	Vector3d vector3_original = vector3;
	Vector3d vector4_original = vector4;

	// apply vector modifier
	double length = 5.458937626;
	VectorTrimmer vector_modifier(length);
	vector_modifier.modify_vector(vector1, Vector3d());
	vector_modifier.modify_vector(vector2, Vector3d());
	vector_modifier.modify_vector(vector3, Vector3d());
	vector_modifier.modify_vector(vector4, Vector3d());

	// first two vectors must be unchanged
	BOOST_CHECK_EQUAL(vector1(0), vector1_original(0));
	BOOST_CHECK_EQUAL(vector1(1), vector1_original(1));
	BOOST_CHECK_EQUAL(vector1(2), vector1_original(2));
	BOOST_CHECK_EQUAL(vector2(0), vector2_original(0));
	BOOST_CHECK_EQUAL(vector2(1), vector2_original(1));
	BOOST_CHECK_EQUAL(vector2(2), vector2_original(2));

	// trimmed down do have specified length (and same direction as original)
	BOOST_CHECK_CLOSE(norm_2(vector3), length, 0.1);
	BOOST_CHECK_CLOSE(vector3(0)/norm_2(vector3), vector3_original(0)/norm_2(vector3_original), 0.1);
	BOOST_CHECK_CLOSE(vector3(1)/norm_2(vector3), vector3_original(1)/norm_2(vector3_original), 0.1);
	BOOST_CHECK_CLOSE(vector3(2)/norm_2(vector3), vector3_original(2)/norm_2(vector3_original), 0.1);

	// unchanged (has exactly the allowed length)
	BOOST_CHECK_EQUAL(vector4(0), vector4_original(0));
	BOOST_CHECK_EQUAL(vector4(1), vector4_original(1));
	BOOST_CHECK_EQUAL(vector4(2), vector4_original(2));
}


/*
 * Test: Trim difference between several vectors and a specified reference vector to zero length.
 */
BOOST_AUTO_TEST_CASE(vector_difference_trimmer_zero_test) {
	using boost::numeric::ublas::zero_vector;
	using boost::numeric::ublas::unit_vector;
	using boost::numeric::ublas::norm_2;
	using boost::numeric::ublas::outer_prod;

	// length: 124.26764
	Vector3d reference_vector;
	reference_vector(0) = 123.934;
	reference_vector(0) = -0.0000123;
	reference_vector(0) = 9.1;

	Vector3d vector1 = zero_vector<double>(3);
	Vector3d vector2 = unit_vector<double>(3, 2);

	// length: 110.6788729
	Vector3d vector3;
	vector3(0) = -102.4;
	vector3(1) = -0.23;
	vector3(2) = 42.;

	// length: 5.458937626
	Vector3d vector4;
	vector4(0) = 3;
	vector4(1) = 4.4;
	vector4(2) = -1.2;

	Vector3d vector5 = reference_vector;

	// copies
	Vector3d vector1_original = vector1;
	Vector3d vector2_original = vector2;
	Vector3d vector3_original = vector3;
	Vector3d vector4_original = vector4;
	Vector3d vector5_original = vector5;

	// apply vector modifier
	double length = 0.;
	VectorDifferenceTrimmer vector_modifier(length);
	vector_modifier.modify_vector(vector1, reference_vector);
	vector_modifier.modify_vector(vector2, reference_vector);
	vector_modifier.modify_vector(vector3, reference_vector);
	vector_modifier.modify_vector(vector4, reference_vector);
	vector_modifier.modify_vector(vector5, reference_vector);

	// all vectors must nearly equal reference vector
	BOOST_CHECK_CLOSE(vector1(0), reference_vector(0), 0.1);
	BOOST_CHECK_CLOSE(vector1(1), reference_vector(1), 0.1);
	BOOST_CHECK_CLOSE(vector1(2), reference_vector(2), 0.1);
	BOOST_CHECK_CLOSE(vector2(0), reference_vector(0), 0.1);
	BOOST_CHECK_CLOSE(vector2(1), reference_vector(1), 0.1);
	BOOST_CHECK_CLOSE(vector2(2), reference_vector(2), 0.1);
	BOOST_CHECK_CLOSE(vector3(0), reference_vector(0), 0.1);
	BOOST_CHECK_CLOSE(vector3(1), reference_vector(1), 0.1);
	BOOST_CHECK_CLOSE(vector3(2), reference_vector(2), 0.1);
	BOOST_CHECK_CLOSE(vector4(0), reference_vector(0), 0.1);
	BOOST_CHECK_CLOSE(vector4(1), reference_vector(1), 0.1);
	BOOST_CHECK_CLOSE(vector4(2), reference_vector(2), 0.1);
	BOOST_CHECK_CLOSE(vector5(0), reference_vector(0), 0.1);
	BOOST_CHECK_CLOSE(vector5(1), reference_vector(1), 0.1);
	BOOST_CHECK_CLOSE(vector5(2), reference_vector(2), 0.1);
}


/*
 * Test: Trim difference between several vectors and a specified reference vector to non-zero length.
 */
BOOST_AUTO_TEST_CASE(vector_difference_trimmer_non_zero_test) {
	using boost::numeric::ublas::zero_vector;
	using boost::numeric::ublas::unit_vector;
	using boost::numeric::ublas::norm_2;
	using boost::numeric::ublas::outer_prod;

	// length: 124.26764
	Vector3d reference_vector;
	reference_vector(0) = 123.934;
	reference_vector(1) = -0.0000123;
	reference_vector(2) = 9.1;

	// difference length: 124.26764 (= length of reference vector)
	Vector3d vector1 = zero_vector<double>(3);

	// difference length: 124.2716636
	Vector3d vector2 = unit_vector<double>(3, 2);

	// length: 110.6788729
	// difference length: 228.7127947
	Vector3d vector3;
	vector3(0) = -102.4;
	vector3(1) = -0.23;
	vector3(2) = 42.;

	// length: 5.458937626
	// difference length: 121.4515643
	Vector3d vector4;
	vector4(0) = 3;
	vector4(1) = 4.4;
	vector4(2) = -1.2;

	// difference length: 0.
	Vector3d vector5 = reference_vector;

	// copies
	Vector3d vector1_original = vector1;
	Vector3d vector2_original = vector2;
	Vector3d vector3_original = vector3;
	Vector3d vector4_original = vector4;
	Vector3d vector5_original = vector5;

	// apply vector modifier
	double length = 124.2716636;
	VectorDifferenceTrimmer vector_modifier(length);
	vector_modifier.modify_vector(vector1, reference_vector);
	vector_modifier.modify_vector(vector2, reference_vector);
	vector_modifier.modify_vector(vector3, reference_vector);
	vector_modifier.modify_vector(vector4, reference_vector);
	vector_modifier.modify_vector(vector5, reference_vector);

	// vector1 stays unchanged
	BOOST_CHECK_EQUAL(vector1(0), vector1_original(0));
	BOOST_CHECK_EQUAL(vector1(1), vector1_original(1));
	BOOST_CHECK_EQUAL(vector1(2), vector1_original(2));

	// vector2 stays unchanged (difference length fits exactly)
	BOOST_CHECK_EQUAL(vector2(0), vector2_original(0));
	BOOST_CHECK_EQUAL(vector2(1), vector2_original(1));
	BOOST_CHECK_EQUAL(vector2(2), vector2_original(2));

	// vector3 is changed
	BOOST_CHECK_CLOSE(vector3(0),  0.9548603560, 0.1);
	BOOST_CHECK_CLOSE(vector3(1), -0.1249766866, 0.1);
	BOOST_CHECK_CLOSE(vector3(2), 26.9762965100, 0.1);

	// vector4 stays unchanged
	BOOST_CHECK_EQUAL(vector4(0), vector4_original(0));
	BOOST_CHECK_EQUAL(vector4(1), vector4_original(1));
	BOOST_CHECK_EQUAL(vector4(2), vector4_original(2));

	// vector5 stays unchanged (difference length 0)
	BOOST_CHECK_EQUAL(vector5(0), vector5_original(0));
	BOOST_CHECK_EQUAL(vector5(1), vector5_original(1));
	BOOST_CHECK_EQUAL(vector5(2), vector5_original(2));
}
