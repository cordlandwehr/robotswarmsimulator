#ifndef CIRCLE_LAYOUTER_H_
#define CIRCLE_LAYOUTER_H_


#include <string>
#include <set>



#include "../Wrapper/boost_graph_wrapper.h"

#include <boost/config/no_tr1/cmath.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/topology.hpp>
#include <boost/static_assert.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/random/linear_congruential.hpp>


#include "../Model/world_information.h"
#include "../Model/world_modifier.h"






class CircleLayouter : public WorldModifier {
public:
	CircleLayouter();

	virtual std::string get_algorithm_id() const;

	virtual std::set< boost::shared_ptr<Request> >
    compute(const boost::shared_ptr<WorldInformation> &world_information);

};




#endif /* CIRCLE_LAYOUTER_H_ */
