/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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
