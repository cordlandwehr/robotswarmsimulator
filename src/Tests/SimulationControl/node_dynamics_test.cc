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

#include "../../Model/world_information.h"
#include "../../Model/robot_data.h"
#include "../../Model/edge.h"
#include "../../Model/robot_identifier.h"

#include "../../Views/abstract_view_factory.h"
#include "../../Views/view.h"
#include "../../Views/view_factory.h"

#include "../../SimulationKernel/uniform_robot_control.h"

#include "../Fixtures/simple_graph_fixture.h"


BOOST_FIXTURE_TEST_CASE(node_dynamics_test, SimpleGraphFixture)
{  
	// setup of view and event handler
	boost::shared_ptr<AbstractViewFactory> view_factory(new ViewFactory<LocalGraphView>());
	boost::shared_ptr<RobotControl> robot_control(new UniformRobotControl(view_factory, 5, initial_world_information));


}