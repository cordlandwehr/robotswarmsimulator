//
//  world_modifier.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 16.07.11.
//

#ifndef WORLD_MODIFIER_H_
#define WORLD_MODIFIER_H_

#include <set>
#include <string>
#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Views/abstract_view_factory.h"
#include "../Requests/request.h"

class WorldModifier {
public:
	WorldModifier();
	virtual ~WorldModifier(){};
        
	virtual std::string get_algorithm_id() const = 0;
	virtual std::set<boost::shared_ptr<Request> >
        compute(const boost::shared_ptr<WorldInformation> &world_information) = 0;
    
protected:
    /* nothing here (yet) */
    
private:
    /* nothing here (yet) */
    
};

#endif /* WORLD_MODIFIER_H_ */