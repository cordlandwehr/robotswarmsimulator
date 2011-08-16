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
