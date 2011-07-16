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


class WorldModifier {
public:
	WorldModifier(){};
	virtual ~WorldModifier();
        
	virtual std::string get_algorithm_id () const = 0;
	virtual std::set<boost::shared_ptr<Request> > compute() = 0;
    
protected:
    /* nothing here (yet?) */
    
private:
	/* nothing here (yet?) */
    
};

#endif /* WORLD_MODIFIER_H_ */