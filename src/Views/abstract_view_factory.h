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
 * abstract_view_factory.h
 *
 *  Created on: 12.01.2009
 *      Author: Daniel
 */

#ifndef ABSTRACT_VIEW_FACTORY_H_
#define ABSTRACT_VIEW_FACTORY_H_

#include <string>
#include <boost/shared_ptr.hpp>

class View;
class WorldInformation;

/**
 * \class ViewFactory
 * Interface for a factory which can produce new View objects.
 */
class AbstractViewFactory {
public:
	AbstractViewFactory() {;}
	virtual ~AbstractViewFactory() {;}

	/**
	* Creates an new instance of a View class. The runtime type depends on the
	* sub type of AbstractViewFactory used.
	* Note: The created instance is already initialized.
	* @param const ref to WorldInformation
	* @return Newly constructed View object
	*/
	virtual boost::shared_ptr<View> create_new_view_instance(const boost::shared_ptr<WorldInformation>& world_information) const = 0;
};

#endif /* ABSTRACT_VIEW_FACTORY_H_ */
