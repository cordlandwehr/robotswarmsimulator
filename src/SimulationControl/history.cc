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
 */

#include "../Model/world_information.h"
#include "history.h"

void History::push_back(boost::shared_ptr<WorldInformation> world_information) {

}

const WorldInformation& History::get_oldest() {
	//TODO(craupach) implement this
	return *(history_.at(0));
}

const WorldInformation& History::get_newest() const {
	return *history_.at(0);
}
