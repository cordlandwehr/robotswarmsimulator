#include "marker_information.h"
#include "../Utilities/unsupported_operation_exception.h"

MarkerInformation::MarkerInformation() {
	// TODO Auto-generated constructor stub

}

MarkerInformation::~MarkerInformation() {
	// TODO Auto-generated destructor stub
}

boost::shared_ptr<MarkerInformation> MarkerInformation::clone() const {
	return boost::shared_ptr<MarkerInformation>(new MarkerInformation(*this));
}

void MarkerInformation::add_data(string var_name, boost::any insert_data) {
	data.insert(pair<string,boost::any>(var_name,insert_data));
}

boost::any MarkerInformation::get_data(string var_name) {
	//get iterator to value corresponding to key var_name
	map<string,boost::any>::const_iterator map_iterator = data.find(var_name);

	//check if var_name exists in given map
	if(map_iterator == data.end()) {
		//var_name doesn't exist in given map
		throw UnsupportedOperationException("Variable "+var_name+" doesn't hold any data.");
	} else {
		//return value of var_name
		return data.find(var_name)->second;
	}
}
