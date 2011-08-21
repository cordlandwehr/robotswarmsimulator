#ifndef RANDOM_LAYOUTER_H_
#define RANDOM_LAYOUTER_H_

#include <string>
#include <set>

#include "../Wrapper/boost_graph_wrapper.h"

#include <boost/smart_ptr.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/random/linear_congruential.hpp>


#include "../Model/world_information.h"
#include "../Model/world_modifier.h"

class RandomLayouter : public WorldModifier {
public:
	RandomLayouter();

	virtual std::string get_algorithm_id() const;

	virtual std::set< boost::shared_ptr<Request> >
    compute(const boost::shared_ptr<WorldInformation> &world_information);

};

#endif /* RANDOM_LAYOUTER_H_ */
