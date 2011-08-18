/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

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
 * marker_information.h
 *
 *  Created on: Jan 7, 2009
 *      Author: martinah
 */
#ifndef MARKER_INFORMATION_H_
#define MARKER_INFORMATION_H_

#include <boost/smart_ptr.hpp>
#include <boost/any.hpp>
#include <vector>
#include <string>
#include <map>


/**
 * \brief This class represents abstract information types that may be appended to any WorldObject.
 *
 * The class provides a map-like interface to store any (variable_name, value) pair. To achieve this, the to
 * be saved data has to be wrapped inside a boost::any object.
 */
class MarkerInformation {
public:
	MarkerInformation();

	~MarkerInformation();


	/**
	 * This method adds data to current marker.
	 * \param var_name		Name of variable (used as key)
	 * \param insert_data	Data to be stored.
	 */
	void add_data(const std::string& var_name, const boost::any& insert_data);

	/**
	 * Removes the given data item from this MarkerInformation. Has no effect if no such data item is stored.
	 */
	void remove_data(const std::string& var_name);

	/**
	 * This method returns data stored in given variable.
	 * \param 	var_name	Return data of this variable.
	 * \return	Data stored in data-map for given variable.
	 */
	const boost::any get_data(const std::string& var_name);
	
	/**
	 * This method return the name set of stored variables.
	 * \return	Set of variable names.
	 */
	const std::vector<std::string> get_keys();

private:
	/**
	 * Contains data of this marker. A string (that denotes 'variable name') is used as a key
	 * and a boost::any as a value for the key (holds custom data)
	 */
	std::map<std::string, boost::any> data_;
};

#endif /* MARKER_INFORMATION_H_ */
