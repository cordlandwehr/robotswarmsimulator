/**
 * \brief	TODO insert description here
 */

#ifndef MARKER_INFORMATION_H_
#define MARKER_INFORMATION_H_

#include <boost/smart_ptr.hpp>
#include <boost/any.hpp>
#include <string>
#include <map>

using namespace std;

// TODO(peter) comment class
class MarkerInformation {
public:
	MarkerInformation();

	~MarkerInformation();


	/**
	 * This method adds data to current marker.
	 * \param var_name		Name of variable (used as key)
	 * \param insert_data	Data to be stored.
	 */
	void add_data(const string& var_name, const boost::any& insert_data);

	/**
	 * This method returns data stored in given variable.
	 * \param 	var_name	Return data of this variable.
	 * \return	Data stored in data-map for given variable.
	 */
	const boost::any get_data(const string& var_name);

private:
	/**
	 * Contains data of this marker. A string (that denotes 'variable name') is used as a key
	 * and a boost::any as a value for the key (holds custom data)
	 */
	map<string, boost::any> data_;
};

#endif /* MARKER_INFORMATION_H_ */
