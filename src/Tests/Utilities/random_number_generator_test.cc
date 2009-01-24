#include <boost/test/unit_test.hpp>

#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>


#include <boost/smart_ptr.hpp>
#include <vector>
#include <iostream>

BOOST_AUTO_TEST_CASE(random_number_generator_test)
{
	  boost::mt19937 rng;                 // produces randomness out of thin air
	                                      // see pseudo-random number generators
	  rng.seed(5); // seed
	  boost::uniform_int<> six(1,6);       // distribution that maps to 1..6
	                                      // see random number distributions
	  boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
	           die(rng, six);             // glues randomness with mapping
	  int x = die();                      // simulate rolling a die

	  std::cout << "Rolled: " << x << std::endl;
}
