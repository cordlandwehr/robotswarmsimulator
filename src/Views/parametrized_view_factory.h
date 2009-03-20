/*
 * view_factory.h
 *
 *  Created on: 11.01.2009
 *      Author: Daniel
 */

#ifndef PARAMETRIZED_VIEW_FACTORY_H_
#define PARAMETRIZED_VIEW_FACTORY_H_

#include "abstract_view_factory.h"
#include "view.h"

/**
 * \class ParametrizedViewFactory
 * Factory which can produce new View objects. The type created is the template type
 * of the factory.
 * The second template type describes the parameter type of the View constructor parameter.
 * Constructor parameter determines the argument each created view class instance will get constructed with.
 */

template<typename T, typename P>
class ParametrizedViewFactory : public AbstractViewFactory {
public:
	ParametrizedViewFactory(const P& argument) : argument_(argument) {;}
	virtual ~ParametrizedViewFactory() {;}

	virtual boost::shared_ptr<View> create_new_view_instance(const boost::shared_ptr<WorldInformation>& world_information) const {
		boost::shared_ptr<View> result(new T(argument_));
		result->init(world_information);
		return result;
	}
private:
	P argument_;
};

#endif /* PARAMETRIZED_VIEW_FACTORY_H_ */
