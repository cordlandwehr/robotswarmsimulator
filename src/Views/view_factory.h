/*
 * view_factory.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef VIEW_FACTORY_H_
#define VIEW_FACTORY_H_

#include <string>
#include <boost/shared_ptr.hpp>

class View;
class WorldInformation;

/**
 * \class ViewFactory
 * Factory which can produce new View objects.
 * Constructor parameter determines which sub class of View should actually be used.
 */
class ViewFactory {
public:
	enum KnownViewClass { VIEW };
public:
	ViewFactory(KnownViewClass view_class);
	~ViewFactory();

	/**
	 * Creates an new instance of a View class. The runtime type depends on the
	 * value ViewFactory was constructed with.
	 * Note: The created instance is already initialized.
	 * @param const ref to WorldInformation
	 * @return Newly constructed View object
	 */
	boost::shared_ptr<View> createNewViewInstance(const WorldInformation& world_information) const;
private:
	KnownViewClass view_class_;
};

#endif /* VIEW_FACTORY_H_ */
