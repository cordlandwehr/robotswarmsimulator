/*
 * view_factory.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef VIEW_FACTORY_H_
#define VIEW_FACTORY_H_

#include "abstract_view_factory.h"
#include "view.h"

/**
 * \class ViewFactory
 * Factory which can produce new View objects. The type created is the template type
 * of the factory.
 * Constructor parameter determines which sub class of View should actually be used.
 */
template<typename T>
class ViewFactory : public AbstractViewFactory {
public:
	ViewFactory() {;}
	virtual ~ViewFactory() {;}

	virtual boost::shared_ptr<View> create_new_view_instance(const WorldInformation& world_information) const {
		boost::shared_ptr<View> result(new T);
		result->init(world_information);
		return result;
	}
};

#endif /* VIEW_FACTORY_H_ */
