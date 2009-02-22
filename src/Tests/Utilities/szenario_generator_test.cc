/*
 * szenario_generator_test.cc
 *
 *  Created on: Feb 15, 2009
 *      Author: phoenixx
 */

#include <boost/test/unit_test.hpp>
#include <boost/unordered_map.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Model/robot_identifier.h"
#include "../../Model/world_information.h"

#include "../../Utilities/parser.h"
#include "../../Utilities/vector_arithmetics.h"
#include "../../Utilities/szenario_generator.h"
#include "../../Utilities/distribution_generator.h"

#include <iostream>

BOOST_AUTO_TEST_CASE( szenario_generator_test )
{
	szenario_generator generator(17);
	generator.init(1000);

	Vector3d tmpVec;
	tmpVec.insert_element(kXCoord,100.0);
	tmpVec.insert_element(kYCoord,500.0);
	tmpVec.insert_element(kZCoord,20.0);
	generator.distribute_robots_uniform(tmpVec);
	generator.distribute_robots_normal(tmpVec, 0.0, 1.0);

	generator.write_to_file();

	std::cout << "42 robots generated" << std::endl;
}
