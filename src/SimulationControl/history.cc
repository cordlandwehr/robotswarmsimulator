/*
 * history.cc
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 *
 *
 */

/**
 * \file Empty implementation of the history to prevent the compiler from complaining
 * TODO(craupach) Semaphores should go in here
 */

#include "../Model/world_information.h"
#include "history.h"

History::History(int size) {
	// create new history with given size
	history_.reset(new boost::circular_buffer<boost::shared_ptr<WorldInformation> > (size));
}

void History::push_back(boost::shared_ptr<WorldInformation> world_information) {
	history_->push_front(world_information);
}

const WorldInformation& History::get_oldest() {
	//TODO(craupach) implement this. Should consume.
	return *history_->back();
}

const WorldInformation& History::get_newest() const {
	return *history_->front();
}
