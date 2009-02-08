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

	//TODO(martinah) Is this method supposed to be virtual?
	~MarkerInformation();

	/**
	 * Clones this object and returns a shared ptr to the cloned object.
	 * typeid(*this) == typeid(*clone)
	 * @return shared ptr to the cloned object
	 */
	boost::shared_ptr<MarkerInformation> clone() const;

	/**
	 * This method adds data to current marker.
	 * \param var_name		Name of variable (used as key)
	 * \param insert_data	Data to be stored.
	 */
	void add_data(string var_name, boost::any insert_data);

	/**
	 * This method returns data stored in given variable.
	 * \param 	var_name	Return data of this variable.
	 * \return	Data stored in data-map for given variable.
	 */
	boost::any get_data(string var_name);

private:
	/**
	 * Contains data of this marker. A string (that denotes 'variable name') is used as a key
	 * and a boost::any as a value for the key (holds custom data)
	 * TODO(martinah) Use boost::shared_ptr<boost::any> instead of just boost::any?
	 */
	map<string, boost::any> data;
};

#endif /* MARKER_INFORMATION_H_ */
