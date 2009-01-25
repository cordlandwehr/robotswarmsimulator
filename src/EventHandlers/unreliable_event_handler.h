/*
 * unreliable_event_handler.h
 *
 *  Created on: 25.01.2009
 *      Author: peter
 */

#ifndef UNRELIABLE_EVENT_HANDLER_H_
#define UNRELIABLE_EVENT_HANDLER_H_

#include <boost/shared_ptr.hpp>

#include "../SimulationKernel/robot_control.h"
#include "../SimulationControl/history.h"

#include "../Model/world_information.h"

#include "../Requests/acceleration_request.h"
#include "../Requests/marker_request.h"
#include "../Requests/position_request.h"
#include "../Requests/type_change_request.h"
#include "../Requests/velocity_request.h"

#include "../Utilities/distribution_generator.h"


/**
 * \brief Template wrapper that can give any other event_handler the property to be unreliable.
 *
 * The unreliability effects acceleration, marker, position, type and velocity requests. The discard_probability
 * parameter defines the probability with that a request will be discarded.
 */
template<typename BASE_EVENT_HANDLER>
class UnreliableEventHandler : public BASE_EVENT_HANDLER {
public:
	UnreliableEventHandler(unsigned int seed, double discard_probability, boost::shared_ptr<History> history,
	                       boost::shared_ptr<RobotControl> robot_control)
	: BASE_EVENT_HANDLER(history, robot_control), distribution_generator_(new DistributionGenerator(seed)),
	  discard_probability_(discard_probability) {
		distribution_generator_->init_bernoulli(discard_probability_);
	};

	// getter/setter for discard probability
	double discard_probability() const { return discard_probability_; }
	void set_discard_probability(double value) {
		discard_probability_ = value;
		distribution_generator_->init_bernoulli(discard_probability_);
	}

protected:
	/**
	 * \brief Discards the request with probability discard_probability_.
	 *
	 * If not discarded, event is handles exactly as by BASE_EVENT_HANDLER.
	 */
	void handle_acceleration_request(boost::shared_ptr<WorldInformation> world_information,
	                                 boost::shared_ptr<const AccelerationRequest> acceleration_request) {
		if (distribution_generator_->get_value_bernoulli())
			return;
		BASE_EVENT_HANDLER::handle_acceleration_request(world_information, acceleration_request);
	}

	/**
	 * \brief Discards the request with probability discard_probability_.
	 *
	 * If not discarded, event is handles exactly as by BASE_EVENT_HANDLER.
	 */
	void handle_marker_request(boost::shared_ptr<WorldInformation> world_information,
	                           boost::shared_ptr<const MarkerRequest> marker_request) {
		if (distribution_generator_->get_value_bernoulli())
			return;
		BASE_EVENT_HANDLER::handle_marker_request(world_information, marker_request);
	}


	/**
	 * \brief Discards the request with probability discard_probability_.
	 *
	 * If not discarded, event is handles exactly as by BASE_EVENT_HANDLER.
	 */
	void handle_position_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const PositionRequest> position_request) {
		if (distribution_generator_->get_value_bernoulli())
			return;
		BASE_EVENT_HANDLER::handle_position_request(world_information, position_request);
	}

	/**
	 * \brief Discards the request with probability discard_probability_.
	 *
	 * If not discarded, event is handles exactly as by BASE_EVENT_HANDLER.
	 */
	void handle_type_change_request(boost::shared_ptr<WorldInformation> world_information,
	                                boost::shared_ptr<const TypeChangeRequest> type_change_request) {
		if (distribution_generator_->get_value_bernoulli())
			return;
		BASE_EVENT_HANDLER::handle_type_change_request(world_information, type_change_request);
	}

	/**
	 * \brief Discards the request with probability discard_probability_.
	 *
	 * If not discarded, event is handles exactly as by BASE_EVENT_HANDLER.
	 */
	void handle_velocity_request(boost::shared_ptr<WorldInformation> world_information,
	                             boost::shared_ptr<const VelocityRequest> velocity_request) {
		if (distribution_generator_->get_value_bernoulli())
			return;
		BASE_EVENT_HANDLER::handle_velocity_request(world_information, velocity_request);
	}

private:

	// member variables used for the unreliability effect
	boost::shared_ptr<DistributionGenerator> distribution_generator_;
	double discard_probability_;
};

#endif /* UNRELIABLE_EVENT_HANDLER_H_ */
