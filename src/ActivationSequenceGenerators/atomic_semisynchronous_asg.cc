#include "atomic_semisynchronous_asg.h"

#include "../Events/event.h"
#include "../Events/look_event.h"
#include "../Events/compute_event.h"
#include "../Events/handle_requests_event.h"
#include "../SimulationControl/history.h"
#include "../Model/robot_data.h"
#include "../Model/robot.h"

void AtomicSemisynchronousASG::initialize(const History& history, const vector<boost::shared_ptr<Robot> >& robots) {

}

boost::shared_ptr<Event> AtomicSemisynchronousASG::get_next_event() {
	boost::shared_ptr<Event> next_event;
	return next_event;
}
