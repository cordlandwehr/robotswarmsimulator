#include <boost/test/unit_test.hpp>
#include "../ComputationalGeometry/diameter.h"
#include <boost/timer.hpp>
#include <stdio.h>

using namespace std;

BOOST_AUTO_TEST_CASE(diameter_test)
{
	std::cout
			<< "Testing: Calculation of the diameter of the convex hull of a point set in the three-dimensional euclidean space"
			<< std::endl << std::endl;

	double epsilon = 0.01;

	std::vector<Vector3d> points;
	std::vector<Vector3d> diametrical_pair;
	double diameter;
	boost::timer timer;
	double time;

	/*********** Test 1 ***********/

	int iterations = 100000;

	Vector3d a;
	a(0) = 1.0;
	a(1) = 4.0;
	a(2) = 10.0;

	Vector3d b;
	b(0) = 2.2;
	b(1) = 3.0;
	b(2) = 0.0;

	Vector3d c;
	c(0) = 2.0;
	c(1) = 9.0;
	c(2) = 2.0;

	Vector3d d;
	d(0) = 7.0;
	d(1) = 6.0;
	d(2) = 3.0;

	points.push_back(a);
	points.push_back(b);
	points.push_back(c);
	points.push_back(d);

	std::cout << "Test 1: Fixed set of 4 vectors" << std::endl;
	printf("Using: Approximation algorithm, %i iterations\n", iterations);

	for (int i = 0; i < iterations; i++) {
		diametrical_pair = DiameterAlgorithms::compute_diametrical_pair(points,
				epsilon);
	}

	time = timer.elapsed();

	diameter = DiameterAlgorithms::compute_diameter(points, epsilon);
	printf("Diameter distance: %g\n", diameter);

	printf("Points realizing the diameter:\n\t(%g, %g, %g) - (%g, %g, %g)\n",
			diametrical_pair.at(0)(0), diametrical_pair.at(0)(1),
			diametrical_pair.at(0)(2), diametrical_pair.at(1)(0),
			diametrical_pair.at(1)(1), diametrical_pair.at(1)(2));

	std::cout << "Elapsed time: " << time << " s" << std::endl << std::endl;

	/*********** Test 2 ***********/

	std::cout << "Test 2: Fixed set of 4 vectors" << std::endl;
	printf("Using: Naive algorithm, %i iterations\n", iterations);

	timer.restart();

	for (int i = 0; i < iterations; i++) {
		diameter = DiameterAlgorithms::compute_diameter_naive(points);
	}

	time = timer.elapsed();

	printf("Diameter distance: %g\n", diameter);
	std::cout << "Elapsed time: " << time << " s" << std::endl << std::endl;

	/*********** Test 3 ***********/

	int n = 10000;

	printf("Test 3: Set of %i randomly selected vectors\n", n);
	std::cout << "Using: Approximation algorithm" << std::endl;

	int seed = 42;
	int M = 1500;
	double r;
	Vector3d v;

	srand(seed);

	points.clear();

	for (int i = 0; i < n; i++) {
		r = ((double) rand() / ((double) (RAND_MAX) + (double) (1))) * M;
		v(0) = r;
		r = ((double) rand() / ((double) (RAND_MAX) + (double) (1))) * M;
		v(1) = r;
		r = ((double) rand() / ((double) (RAND_MAX) + (double) (1))) * M;
		v(2) = r;
		points.push_back(v);
	}

	timer.restart();

	diametrical_pair = DiameterAlgorithms::compute_diametrical_pair(points,
			epsilon);

	time = timer.elapsed();

	printf("Diameter distance: %g\n", vector3d_distance(diametrical_pair.at(0),
			diametrical_pair.at(1)));

	printf("Points realizing the diameter:\n\t(%g, %g, %g) - (%g, %g, %g)\n",
			diametrical_pair.at(0)(0), diametrical_pair.at(0)(1),
			diametrical_pair.at(0)(2), diametrical_pair.at(1)(0),
			diametrical_pair.at(1)(1), diametrical_pair.at(1)(2));

	std::cout << "Elapsed time: " << time << " s" << std::endl << std::endl;

	/*********** Test 4 ***********/

	printf("Test 4: Set of %i randomly selected vectors\n", n);
	std::cout << "Using: Naive algorithm" << std::endl;

	timer.restart();

	diameter = DiameterAlgorithms::compute_diameter_naive(points);

	time = timer.elapsed();

	printf("Diameter distance: %g\n", diameter);
	std::cout << "Elapsed time: " << time << " s" << std::endl;
}
