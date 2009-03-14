#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>

#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationControl/history.h"
#include "../../Model/robot_data.h"

#include "../../Model/world_information.h"

BOOST_AUTO_TEST_CASE(simkernel_init) {

	// building SimKernel
	boost::shared_ptr<SimulationKernel> simkernel(new SimulationKernel());
	boost::shared_ptr<History> history(new History(42));
	//TODO(craupach) fails horribly for me atm.
	simkernel->init("src/Tests/TestData/testfile_2", history, "", true);

	// get World-Information
	boost::shared_ptr<WorldInformation> world_info(new WorldInformation(history->get_newest()));

	// get Robotdata
	std::vector< boost::shared_ptr<RobotData> > robots_data = world_info->robot_data();

	/* check data for 1st robot:
 		id: 0
		position: 0.0, 0.0, 0.0
		type: "slave"
		velocity: 0.0, 0.0, 0.0
		acceleration: 0.0, 0.0, 0.0
		status: "ready"
		marker-info: 0
		algorithm: "COG"
		color: 0
		x-axis: 2.0, 0.0, 0.0
		y-axis: 0.0, 2.0, 0.0
		z-axis: 0.0, 0.0, 2.0		*/

	BOOST_CHECK_EQUAL(robots_data[0]->position()[kXCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[0]->position()[kYCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[0]->position()[kZCoord], 0.0);

	BOOST_CHECK_EQUAL(robots_data[0]->type(), SLAVE);

	BOOST_CHECK_EQUAL(robots_data[0]->velocity()[kXCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[0]->velocity()[kYCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[0]->velocity()[kZCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[0]->acceleration()[kXCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[0]->acceleration()[kYCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[0]->acceleration()[kZCoord], 0.0);

	BOOST_CHECK_EQUAL(robots_data[0]->status(), READY);

	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[0]->coordinate_system_axis()))(kXCoord), 2.0);
	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[0]->coordinate_system_axis()))(kYCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[0]->coordinate_system_axis()))(kZCoord), 0.0);

	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[0]->coordinate_system_axis()))(kXCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[0]->coordinate_system_axis()))(kYCoord), 2.0);
	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[0]->coordinate_system_axis()))(kZCoord), 0.0);

	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[0]->coordinate_system_axis()))(kXCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[0]->coordinate_system_axis()))(kYCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[0]->coordinate_system_axis()))(kZCoord), 2.0);


	/* check data for 2nd robot:
 		id: 1
		position: 1.0, 0.5, 3.0
		type: "slave"
		velocity: 0.0, 0.0, 0.0
		acceleration: 0.0, 0.0, 0.0
		status: "ready"
		marker-info: 0
		algorithm: "COG"
		color: 0
		x-axis: 1.0, 0.0, 0.0
		y-axis: 0.0, 1.0, 0.0
		z-axis: 0.0, 0.0, 1.0		*/

	BOOST_CHECK_EQUAL(robots_data[1]->position()[kXCoord], 1.0);
	BOOST_CHECK_EQUAL(robots_data[1]->position()[kYCoord], 0.5);
	BOOST_CHECK_EQUAL(robots_data[1]->position()[kZCoord], 3.0);

	BOOST_CHECK_EQUAL(robots_data[1]->type(), SLAVE);

	BOOST_CHECK_EQUAL(robots_data[1]->velocity()[kXCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[1]->velocity()[kYCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[1]->velocity()[kZCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[1]->acceleration()[kXCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[1]->acceleration()[kYCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[1]->acceleration()[kZCoord], 0.0);

	BOOST_CHECK_EQUAL(robots_data[1]->status(), READY);

	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[1]->coordinate_system_axis()))(kXCoord), 1.0);
	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[1]->coordinate_system_axis()))(kYCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[1]->coordinate_system_axis()))(kZCoord), 0.0);

	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[1]->coordinate_system_axis()))(kXCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[1]->coordinate_system_axis()))(kYCoord), 1.0);
	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[1]->coordinate_system_axis()))(kZCoord), 0.0);

	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[1]->coordinate_system_axis()))(kXCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[1]->coordinate_system_axis()))(kYCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[1]->coordinate_system_axis()))(kZCoord), 1.0);

	/* check data for 3rd robot:
 		id: 2
		position: 1.0, 1.0, 1.0
		type: "slave"
		velocity: 0.0, 0.0, 0.0
		acceleration: 0.0, 0.0, 0.0
		status: "ready"
		marker-info: 0
		algorithm: "COG"
		color: 0
		x-axis: 1.5, 1.0, 0.0
		y-axis: 0.0, 1.9, 1.0
		z-axis: 1.8, 0.0, 1.3		*/

	BOOST_CHECK_EQUAL(robots_data[2]->position()[kXCoord], 1.0);
	BOOST_CHECK_EQUAL(robots_data[2]->position()[kYCoord], 1.0);
	BOOST_CHECK_EQUAL(robots_data[2]->position()[kZCoord], 1.0);

	BOOST_CHECK_EQUAL(robots_data[2]->type(), SLAVE);

	BOOST_CHECK_EQUAL(robots_data[2]->velocity()[kXCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[2]->velocity()[kYCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[2]->velocity()[kZCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[2]->acceleration()[kXCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[2]->acceleration()[kYCoord], 0.0);
	BOOST_CHECK_EQUAL(robots_data[2]->acceleration()[kZCoord], 0.0);

	BOOST_CHECK_EQUAL(robots_data[2]->status(), READY);

	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[2]->coordinate_system_axis()))(kXCoord), 1.5);
	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[2]->coordinate_system_axis()))(kYCoord), 1.0);
	BOOST_CHECK_EQUAL((*boost::get<0>(robots_data[2]->coordinate_system_axis()))(kZCoord), 0.0);

	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[2]->coordinate_system_axis()))(kXCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[2]->coordinate_system_axis()))(kYCoord), 1.9);
	BOOST_CHECK_EQUAL((*boost::get<1>(robots_data[2]->coordinate_system_axis()))(kZCoord), 1.0);

	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[2]->coordinate_system_axis()))(kXCoord), 1.8);
	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[2]->coordinate_system_axis()))(kYCoord), 0.0);
	BOOST_CHECK_EQUAL((*boost::get<2>(robots_data[2]->coordinate_system_axis()))(kZCoord), 1.3);

	//check for vectormodifiers
	//TODO(mmarcus)

}
