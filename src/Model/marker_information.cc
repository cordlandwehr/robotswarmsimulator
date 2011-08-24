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

#include "marker_information.h"
#include <stdexcept>

MarkerInformation::MarkerInformation() {

}

MarkerInformation::~MarkerInformation() {

}

void MarkerInformation::add_data(const std::string& var_name, const boost::any& insert_data) {
	data_[var_name] = insert_data;
}

void MarkerInformation::remove_data(const std::string& var_name) {
	data_.erase(var_name);
}

const boost::any MarkerInformation::get_data(const std::string& var_name) {
	//get iterator to value corresponding to key var_name
	std::map<std::string,boost::any>::const_iterator map_iterator = data_.find(var_name);

	//check if var_name exists in given map
	if(map_iterator == data_.end()) {
		//var_name doesn't exist in given map
		throw std::invalid_argument("Variable "+var_name+" doesn't hold any data.");
	} else {
		//return value of var_name
		return map_iterator->second;
	}
}

const std::vector<std::string> MarkerInformation::get_keys() {
  // create result set
  std::vector<std::string> result;
  
  // iterate over map and add keys to set
  std::map<std::string, boost::any>::const_iterator it;
  for (it = data_.begin(); it != data_.end(); it++) {
    result.push_back(it->first);
  }
   
  // return set
  return result;
}

bool MarkerInformation::has_key(const std::string& key) {
  std::map<std::string, boost::any>::const_iterator it;
  it = data_.find(key);
  return (it != data_.end());
}
