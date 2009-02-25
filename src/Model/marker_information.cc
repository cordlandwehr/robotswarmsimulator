#include "marker_information.h"
#include <stdexcept>

MarkerInformation::MarkerInformation() {

}

MarkerInformation::~MarkerInformation() {

}

void MarkerInformation::add_data(const string& var_name, const boost::any& insert_data) {
	data_[var_name] = insert_data;
}

void MarkerInformation::remove_data(const string& var_name) {
	data_.erase(var_name);
}

const boost::any MarkerInformation::get_data(const string& var_name) {
	//get iterator to value corresponding to key var_name
	map<string,boost::any>::const_iterator map_iterator = data_.find(var_name);

	//check if var_name exists in given map
	if(map_iterator == data_.end()) {
		//var_name doesn't exist in given map
		throw std::invalid_argument("Variable "+var_name+" doesn't hold any data.");
	} else {
		//return value of var_name
		return map_iterator->second;
	}
}
