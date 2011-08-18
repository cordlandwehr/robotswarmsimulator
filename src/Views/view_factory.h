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

#ifndef VIEW_FACTORY_H_
#define VIEW_FACTORY_H_

#include "abstract_view_factory.h"
#include "view.h"

/**
 * \class ViewFactory
 * Factory which can produce new View objects. The type created is the template type
 * of the factory.
 */
template<typename T>
class ViewFactory : public AbstractViewFactory {
public:
	ViewFactory() {;}
	virtual ~ViewFactory() {;}

	virtual boost::shared_ptr<View> create_new_view_instance(const boost::shared_ptr<WorldInformation>& world_information) const {
		boost::shared_ptr<View> result(new T);
		result->init(world_information);
		return result;
	}
};
#endif /* VIEW_FACTORY_H_ */
