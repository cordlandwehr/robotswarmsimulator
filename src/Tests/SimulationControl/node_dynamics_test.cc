/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>

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
#include <boost/smart_ptr.hpp>
#include <boost/program_options.hpp>
#include <iostream>

#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationControl/history.h"

#include "../../RobotImplementations/simple_robot.h"

#include "../../Model/world_information.h"
#include "../../Model/robot_data.h"
#include "../../Model/edge.h"
#include "../../Model/robot_identifier.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"


BOOST_AUTO_TEST_CASE(node_dynamics_test) {

	// building SimKernel
	boost::shared_ptr<SimulationKernel> simkernel(new SimulationKernel());
	boost::shared_ptr<History> history(new History(42));
	simkernel->init("../../../src/Tests/TestData/testfile_2", history, "");

	// get World-Information
	boost::shared_ptr<WorldInformation> world_info(new WorldInformation(history->get_newest().world_information()));

	// check for adding robot
	BOOST_CHECK_EQUAL(world_info->robot_data().size(), 2);
	BOOST_CHECK_EQUAL(world_info->edges().size(), 2);
	
	//new robot	
	boost::shared_ptr<RobotIdentifier> rid6 (new RobotIdentifier(6));
	const SimpleRobot r6 (rid6);
	boost::shared_ptr<RobotData> rd1(new RobotData(rid6, boost::shared_ptr<Vector3d>(new Vector3d()), r6));
	
	//retrieve old robot reference (robot 0)
	boost::shared_ptr<RobotIdentifier> rid0 = boost::dynamic_pointer_cast<RobotIdentifier>(world_info->robot_data()[0]->id()); 
	
	//new edge
	boost::shared_ptr<DirectedEdge> e1(new DirectedEdge(rid6, rid0));
	
	world_info->add_robot_data(rd1);
	BOOST_CHECK_EQUAL(world_info->robot_data().size(), 3);
	world_info->add_edge(boost::dynamic_pointer_cast<Edge>(e1));
	BOOST_CHECK_EQUAL(world_info->edges().size(), 3);
	
	//note: not adding the edges to the actual robot_datas because this is just for testing...
	
	//check for removing robot
	world_info->remove_robot_data(rid6);
	BOOST_CHECK_EQUAL(world_info->robot_data().size(), 2);
	BOOST_CHECK_EQUAL(world_info->edges().size(), 2);
	
}
