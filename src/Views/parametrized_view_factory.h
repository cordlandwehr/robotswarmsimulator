/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * view_factory.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef PARAMETRIZED_VIEW_FACTORY_H_
#define PARAMETRIZED_VIEW_FACTORY_H_

#include "abstract_view_factory.h"
#include "view.h"

/**
 * \class ParametrizedViewFactory
 * Factory which can produce new View objects. The type created is the template type
 * of the factory.
 * The second template type describes the parameter type of the View constructor parameter.
 * Constructor parameter determines the argument each created view class instance will get constructed with.
 */

template<typename T, typename P>
class ParametrizedViewFactory : public AbstractViewFactory {
public:
	ParametrizedViewFactory(const P& argument) : argument_(argument) {;}
	virtual ~ParametrizedViewFactory() {;}

	virtual boost::shared_ptr<View> create_new_view_instance(const boost::shared_ptr<WorldInformation>& world_information) const {
		boost::shared_ptr<View> result(new T(argument_));
		result->init(world_information);
		return result;
	}
private:
	P argument_;
};

#endif /* PARAMETRIZED_VIEW_FACTORY_H_ */
