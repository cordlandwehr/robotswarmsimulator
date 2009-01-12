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
	virtual boost::shared_ptr<View> create_new_view_instance(const WorldInformation& world_information) const = 0;
};

#endif /* ABSTRACT_VIEW_FACTORY_H_ */
