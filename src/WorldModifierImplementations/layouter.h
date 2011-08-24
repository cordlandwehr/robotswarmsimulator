//
//  layouter.h
//  Robot Swarm Simulator
//
//  Created by xynodes on 24.08.11.
//

#ifndef LAYOUTER_H_
#define LAYOUTER_H_

#include <boost/smart_ptr.hpp>


#include "../Model/world_modifier.h"
#include "../Wrapper/boost_graph_wrapper.h"

class WorldInformation;

class Layouter : public WorldModifier{
public:
	static boost::shared_ptr<BoostGraph> transform_into_boost_graph(const WorldInformation& world_information);

	static void apply_positions(WorldInformation& world_information, const BoostGraph& boost_graph);
};

#endif /* LAYOUTER_H_ */
