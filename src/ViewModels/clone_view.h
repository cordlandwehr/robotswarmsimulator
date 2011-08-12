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
 * clone_view.h
 *
 *  Created on: 18.04.2009
 *      Author: Daniel
 */

#ifndef CLONE_VIEW_H_
#define CLONE_VIEW_H_

#include "../Views/spheric_view.h"
#include "../Views/id_view.h"
#include "../Views/time_view.h"
#include "../Views/position_view.h"
#include "../Views/velocity_view.h"

class CloneView: public virtual SphericView,
		public virtual IdView,
		public virtual TimeView,
		public virtual PositionView,
		public virtual VelocityView {
public:
	CloneView(double view_radius);
	virtual ~CloneView();
};

#endif /* CLONE_VIEW_H_ */
