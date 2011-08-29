//
//  test_world_modifier.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 19.07.11.
//

#include <boost/foreach.hpp>

#include "../Model/world_information.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Requests/position_request.h"
#include "../Utilities/console_output.h"
#include "../Utilities/vector_arithmetics.h"

#include "test_world_modifier.h"

std::string TestWorldModifier::get_algorithm_id() const{
    return "TEST_WORLD_MODIFIER";
}

std::set< boost::shared_ptr<Request> > 
    TestWorldModifier::compute(const boost::shared_ptr<WorldInformation> &world_information) {
    std::set< boost::shared_ptr<Request> > requests;


	
	for (std::map< int, boost::shared_ptr < RobotData> >::const_iterator it =  world_information->robot_data().begin(); it != world_information->robot_data().end(); ++it) {
		boost::shared_ptr<RobotData> robot = it->second;
        
        Vector3d* test = new Vector3d();
        *test = -robot->position();
        boost::shared_ptr<Vector3d> pos(test);
        
        boost::shared_ptr<PositionRequest>
            request(new PositionRequest(const_cast<Robot&>(robot->robot()), pos));
        requests.insert(request);
    }
    
    return requests;
}