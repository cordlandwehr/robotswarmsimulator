/*
 * one_point_formation_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef ONE_POINT_FORMATION_VIEW_H_
#define ONE_POINT_FORMATION_VIEW_H_

#include "../Views/velocity_view.h"
#include "../Views/spheric_view.h"
#include "../Views/acceleration_view.h"
#include "../Views/position_view.h"

/**
 * \class OnePointFormationView
 * \brief View model of the 1PointFormation algorithm
 *
 * Assigning this class to a Robot corresponds to the 1 Point Formation view model, i.e. every Robot can see every other Robots
 * position, velocity and acceleration only in a limited radius. The coordinate-system and id of each Robot is not visible.
 *
 * @see https://wiki.math.uni-paderborn.de/pg-schwarm/StartSeite/AK/Szenarien
 *
 */

class OnePointFormationView: public virtual VelocityView,
		public virtual SphericView,
		public virtual AccelerationView,
		public virtual PositionView {
public:
	OnePointFormationView(double view_radius);
	virtual ~OnePointFormationView();
};

#endif /* ONE_POINT_FORMATION_VIEW_H_ */
