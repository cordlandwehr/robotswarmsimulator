/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
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
 * distribution_generator.cc
 *
 *  Created on: Jan 23, 2009
 *      Author: phoenixx
 */

#include "distribution_generator.h"
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_on_sphere.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/random/geometric_distribution.hpp>
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_real.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include "vector_arithmetics.h"

void DistributionGenerator::init_uniform(int min, int max) {
	boost::uniform_int<> range(min,max);
	gen_uniform_int_ = boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::uniform_int<> > >
		(new boost::variate_generator <boost::mt19937&, boost::uniform_int<> > (png_mersenne_, range));
}


int DistributionGenerator::get_value_uniform() {
	return (*gen_uniform_int_)() ;
}

void DistributionGenerator::init_normal(double mean, double sigma) {
	/* p(x) = 1/sqrt(2*pi*sigma) * exp(- (x-mean)^2 / (2*sigma^2) ) */
	boost::normal_distribution<> params(mean,sigma);	// parameter for normal distribution
	gen_normal_ = boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > >
		(new boost::variate_generator <boost::mt19937&, boost::normal_distribution<> > (png_mersenne_, params));
}

double DistributionGenerator::get_value_normal() {
	return (*gen_normal_)();
}

void DistributionGenerator::init_bernoulli(double p) {
	boost::bernoulli_distribution<> probability(p);
	gen_bernoulli_ = boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::bernoulli_distribution<> > >
		(new boost::variate_generator <boost::mt19937&, boost::bernoulli_distribution<> > (png_mersenne_, probability));
}

bool DistributionGenerator::get_value_bernoulli() {
	return (*gen_bernoulli_)();
}

void DistributionGenerator::init_exponential(double lambda) {
	boost::exponential_distribution<> params(lambda);
	gen_exponential_ = boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::exponential_distribution<> > >
		(new boost::variate_generator <boost::mt19937&, boost::exponential_distribution<> > (png_mersenne_, params));
}

double DistributionGenerator::get_value_exponential() {
	return (*gen_exponential_)() ;
}

void DistributionGenerator::init_geometric(double p) {
	boost::geometric_distribution<> params(p);
	gen_geometric_.reset(new boost::variate_generator <boost::mt19937&, boost::geometric_distribution<> > (png_mersenne_, params));
}

int DistributionGenerator::get_value_geometric() {
	return (*gen_geometric_)();
}

void DistributionGenerator::init_uniform_real(double min, double max) {
	boost::uniform_real<> range(min,max);
	gen_uniform_real_ = boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::uniform_real<> > >
		(new boost::variate_generator <boost::mt19937&, boost::uniform_real<> > (png_mersenne_, range));
}

double DistributionGenerator::get_value_uniform_real() {
	return (*gen_uniform_real_)();
}

void DistributionGenerator::init_uniform_on_sphere(int dim) {
	boost::uniform_on_sphere<> params(dim);
	gen_uniform_on_sphere_ = boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::uniform_on_sphere<> > >
		(new boost::variate_generator <boost::mt19937&, boost::uniform_on_sphere<> > (png_mersenne_, params));
}

std::vector<double> DistributionGenerator::get_value_uniform_on_sphere() {
	return (*gen_uniform_on_sphere_)();
}

Vector3d DistributionGenerator::get_value_uniform_on_sphere_3d() {
	Vector3d randomVector3d;
	randomVector3d.insert_element(kXCoord,0.0);
	randomVector3d.insert_element(kYCoord,0.0);
	randomVector3d.insert_element(kZCoord,0.0);

	std::vector<double> randomVector = (*gen_uniform_on_sphere_)();
	std::vector<double>::iterator iter;

	int currentDimension = 0;
	for(iter=randomVector.begin(); iter != randomVector.end(); iter++ ) {
		randomVector3d.insert_element(currentDimension++, *iter);
	}
	return randomVector3d;
}

void DistributionGenerator::set_seed(unsigned int seed) {
	png_mersenne_.seed(static_cast<boost::mt19937::result_type>(seed)); // int is not possible
}

DistributionGenerator::DistributionGenerator(unsigned int seed) {
	set_seed(seed);
}

DistributionGenerator::~DistributionGenerator() {

}
