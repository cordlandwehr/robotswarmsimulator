/*
 * sphere_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "sphere_view.h"
#include "../Model/sphere.h"

SphereView::SphereView() {

}

SphereView::~SphereView() {

}

double SphereView::get_sphere_radius(const Sphere& sphere) const {
	return sphere.radius();
}
