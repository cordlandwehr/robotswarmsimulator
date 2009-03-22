/*
 * time_view.cc
 *
 *  Created on: 22.03.2009
 *      Author: Daniel Wonisch
 */

#include <Views/time_view.h>
#include <Model/world_information.h>

TimeView::TimeView() {

}

TimeView::~TimeView() {

}

const int TimeView::get_time() const {
	return world_information().time();
}


