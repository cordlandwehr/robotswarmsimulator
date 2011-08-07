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