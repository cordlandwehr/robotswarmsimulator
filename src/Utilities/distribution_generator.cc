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
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_real.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

void DistributionGenerator::init_uniform(int min, int max) {
	boost::uniform_int<> range(min,max);
	gen_uniform_int_ = boost::shared_ptr < boost::variate_generator<boost::mt19937, boost::uniform_int<> > >
		(new boost::variate_generator <boost::mt19937, boost::uniform_int<> > (png_mersenne_, range));
}


int DistributionGenerator::get_value_uniform() {
	return (*gen_uniform_int_)() ;
}

void DistributionGenerator::init_normal(double mean, double sigma) {
	/* p(x) = 1/sqrt(2*pi*sigma) * exp(- (x-mean)^2 / (2*sigma^2) ) */
	boost::normal_distribution<> params(mean,sigma);	// parameter for normal distribution
	gen_normal_ = boost::shared_ptr < boost::variate_generator<boost::mt19937, boost::normal_distribution<> > >
		(new boost::variate_generator <boost::mt19937, boost::normal_distribution<> > (png_mersenne_, params));
}

double DistributionGenerator::get_value_normal() {
	return (*gen_normal_)();
}

void DistributionGenerator::init_bernoulli(double p) {
	boost::bernoulli_distribution<> probability(p);
	gen_bernoulli_ = boost::shared_ptr < boost::variate_generator<boost::mt19937, boost::bernoulli_distribution<> > >
		(new boost::variate_generator <boost::mt19937, boost::bernoulli_distribution<> > (png_mersenne_, probability));
}

bool DistributionGenerator::get_value_bernoulli() {
	return (*gen_bernoulli_)();
}

void DistributionGenerator::init_exponential(double lambda) {
	boost::exponential_distribution<> params(lambda);
	gen_exponential_ = boost::shared_ptr < boost::variate_generator<boost::mt19937, boost::exponential_distribution<> > >
		(new boost::variate_generator <boost::mt19937, boost::exponential_distribution<> > (png_mersenne_, params));
}

double DistributionGenerator::get_value_exponential() {
	return (*gen_exponential_)() ;
}

void DistributionGenerator::init_uniform_real(double min, double max) {
	boost::uniform_real<> range(min,max);
	gen_uniform_real_ = boost::shared_ptr < boost::variate_generator<boost::mt19937, boost::uniform_real<> > >
		(new boost::variate_generator <boost::mt19937, boost::uniform_real<> > (png_mersenne_, range));
}

double DistributionGenerator::get_value_uniform_real() {
	return (*gen_uniform_real_)();
}

void DistributionGenerator::init_uniform_on_sphere(int dim) {
	boost::uniform_on_sphere<> params(dim);
	gen_uniform_on_sphere_ = boost::shared_ptr < boost::variate_generator<boost::mt19937, boost::uniform_on_sphere<> > >
		(new boost::variate_generator <boost::mt19937, boost::uniform_on_sphere<> > (png_mersenne_, params));
}

std::vector<double> DistributionGenerator::get_value_uniform_in_sphere() {
	return (*gen_uniform_on_sphere_)();
}

DistributionGenerator::DistributionGenerator(int seed) {
	png_mersenne_.seed(seed);
}

DistributionGenerator::~DistributionGenerator() {
	// TODO Auto-generated destructor stub
}
