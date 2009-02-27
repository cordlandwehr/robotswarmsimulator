/*
 * cog_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef COG_VIEW_H_
#define COG_VIEW_H_

#include "../Views/acceleration_view.h"
#include "../Views/full_view.h"
#include "../Views/position_view.h"
#include "../Views/velocity_view.h"

/**
 * \class CogView
 * \brief View model of the classic CenterOfGravity algorithm
 *
 * Assigning this class to a Robot corresponds to the COG view model, i.e. every Robot can see every other Robots
 * position, velocity and acceleration. The coordinate-system and id of each Robot is not visible.
 *
 * @see https://wiki.math.uni-paderborn.de/pg-schwarm/StartSeite/AK/Szenarien
 *
 */

class CogView: public virtual AccelerationView,
		public virtual FullView,
		public virtual PositionView,
		public virtual VelocityView {
public:
	CogView();
	virtual ~CogView();
};

#endif /* COG_VIEW_H_ */
