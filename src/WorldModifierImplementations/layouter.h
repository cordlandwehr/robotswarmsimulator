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
	/**
	 * Transforms a world information object into a boost graph.
	 *
	 * This method creates a mapping between the nodes in both graphs. The according references are stored in the vertex_descriptor_ attribute of the robot_data objects.
	 *
	 * \param reference to a world information
	 * \return A pointer to the created boost graph
	 */
	static boost::shared_ptr<BoostGraph> transform_into_boost_graph(const WorldInformation& world_information);

	/**
	 * Transfers the positions stored in a boost graph back to the original graph
	 *
	 * This method uses the mapping between nodes that was established during the previous call of transform_into_boost_graph.
	 *
	 * \param const reference to a boost graph
	 */
	static void apply_positions(const BoostGraph& boost_graph);
};

#endif /* LAYOUTER_H_ */
