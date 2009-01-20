/*
 * sphere_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef SPHERE_VIEW_H_
#define SPHERE_VIEW_H_

#include "view.h"

class SphereView: public virtual View {
public:
	SphereView();
	virtual ~SphereView();
protected:
	virtual double get_sphere_radius(const Sphere& sphere) const;
};

#endif /* SPHERE_VIEW_H_ */
