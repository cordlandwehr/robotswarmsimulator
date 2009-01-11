/*
 * view_factory.cc
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#include "view_factory.h"

#include "view.h"

ViewFactory::ViewFactory(KnownViewClass view_class) : view_class_(view_class) {
	;
}

ViewFactory::~ViewFactory() {

}

boost::shared_ptr<View> ViewFactory::createNewViewInstance(const WorldInformation& world_information) const{
	boost::shared_ptr<View> view;
	switch(view_class_) {
	case VIEW:
		view.reset(new View);
		break;
	default:
		throw std::invalid_argument("Unknown View type.");
	}
	view->init(world_information);
	return view;
}
