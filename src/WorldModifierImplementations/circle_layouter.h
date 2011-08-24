//
//  circle_layouter.h
//  Robot Swarm Simulator
//
//  Created by xynodes on 24.08.11.
//

#ifndef CIRCLE_LAYOUTER_H_
#define CIRCLE_LAYOUTER_H_

#include <string>
#include <set>

#include <boost/smart_ptr.hpp>

#include "layouter.h"
#include "../Wrapper/boost_graph_wrapper.h"

class WorldInformation;

class CircleLayouter : public Layouter {
public:
	CircleLayouter();

	virtual std::string get_algorithm_id() const;

	virtual std::set< boost::shared_ptr<Request> >
    compute(const boost::shared_ptr<WorldInformation> &world_information);

};

#endif /* CIRCLE_LAYOUTER_H_ */
