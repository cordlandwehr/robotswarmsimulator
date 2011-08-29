//
//  random_layouter.h
//  Robot Swarm Simulator
//
//  Created by xynodes on 24.08.11.
//

#ifndef RANDOM_LAYOUTER_H_
#define RANDOM_LAYOUTER_H_


#include <string>
#include <set>

#include <boost/smart_ptr.hpp>

#include "layouter.h"
#include "../Wrapper/boost_graph_wrapper.h"

class RandomLayouter : public Layouter {
public:
	RandomLayouter();

	virtual std::string get_algorithm_id() const;

	virtual std::set< boost::shared_ptr<Request> >
    compute(const boost::shared_ptr<WorldInformation> &world_information);

};

#endif /* RANDOM_LAYOUTER_H_ */
