/*
 * distribution_generator.h
 *
 *  Created on: Jan 23, 2009
 *      Author: phoenixx
 */

#ifndef DISTRIBUTION_GENERATOR_H_
#define DISTRIBUTION_GENERATOR_H_

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

/**
 * This class provides different random number generators for different distributions.
 * Additional generators for specific distributions should be defined here.
 */
class DistributionGenerator {
public:

	/**
	 * Initializes variate_generator for uniform distribution
	 * \param min int value for integer range
	 * \param max int value for integer range
	 */
	void init_uniform(int min, int max);

	/**
	 * Generates pseudorandom number in according range
	 * \return random int in range
	 */
	int get_value_uniform();

	/**
	 * Initializes variate_generator for normal distribution
	 * \f[
	 *     p(x) = 1/sqrt(2*pi*sigma) * exp(- (x-mean)^2 / (2*sigma^2) )
	 * \f]
	 * \param mean double value for normal distribution
	 * \param sigma double value for normal distribution
	 */
	void init_normal(double mean, double sigma);

	/**
	 * Generates pseudorandom number for normal distribution
	 * \return random double value distributed according normal distribution
	 */
	double get_value_normal();

	/**
	 * Initializes variate_generator for bernoulli distribution
	 * P(true) = p and P(false) = 1-p
	 * @param probability double for bernoulli distribution in range [0,1)
	 */
	void init_bernoulli(double probability);

	/**
	 * Generates boolen values according to distribution
	 * @return true/false boolean value
	 */
	bool get_value_bernoulli();

	/**
	 * Initializes variate_generator for exponential distribution
	 * \f[
	 *     p(x) = lambda * exp(-lambda * x)
	 * \f]
	 * @param lambda double parameter for distribution, in range [0,1)
	 */
	void init_exponential(double lambda);

	/**
	 * Generates values according to distribution
	 * @return double value
	 */
	double get_value_exponential();

	/**
	 * Initializes variate_generator for normal distribution over the reals
	 * @param min double value of range
	 * @param max double value of range
	 */
	void init_uniform_real(double min, double max);

	/**
	 * Generates values according to distribution
	 * @return double value
	 */
	double get_value_uniform_real();

	/**
	 * Initializes variate_generator for uniform distribution on 3-dimensional unit sphere
	 * @param dimensions
	 */
	void init_uniform_on_sphere(int dim);

	/**
	 * Generates vector according to distribution
	 * @return vector of doubles
	 */
	std::vector<double> get_value_uniform_in_sphere();

	/**
	 * Sets the seed vor PNG
	 * @param seed must be unsigned int
	 */
	void set_seed(unsigned int seed);

	/**
	 * Constructor
	 * \param int seed for pseudorandom number generator
	 */
	DistributionGenerator(unsigned int seed);
	virtual ~DistributionGenerator();

private:
	// distributions
	boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::uniform_int<> > > gen_uniform_int_;
	boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > > gen_normal_;
	boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::bernoulli_distribution<> > > gen_bernoulli_;
	boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::exponential_distribution<> > > gen_exponential_;
	boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::uniform_real<> > > gen_uniform_real_;
	boost::shared_ptr < boost::variate_generator<boost::mt19937&, boost::uniform_on_sphere<> > > gen_uniform_on_sphere_;

	// PNG
	boost::mt19937 png_mersenne_;
};

#endif /* DISTRIBUTION_GENERATOR_H_ */
