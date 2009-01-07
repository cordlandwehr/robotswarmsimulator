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

boost::shared_ptr<WorldInformation> History::get_oldest() {
	boost::shared_ptr<WorldInformation> ptr;
	return ptr;
}
