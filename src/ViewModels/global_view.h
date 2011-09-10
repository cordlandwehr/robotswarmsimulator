/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
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
 * global_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef GLOBAL_VIEW_H_
#define GLOBAL_VIEW_H_

#include "../Views/full_view.h"
#include "../Views/id_view.h"
#include "../Views/marker_information_view.h"
#include "../Views/position_view.h"
#include "../Views/robot_status_view.h"
#include "../Views/robot_last_request_successful_view.h"
#include "../Views/time_view.h"
#include "../Views/message_view.h"

/**
 * \class GlobalView
 * \brief Global information view model.
 *
 * Assigning this class to a Robot corresponds to a "see everything, everywhere" model.
 *
 */

class GlobalView: 
		public virtual FullView,
		public virtual IdView,
		public virtual MarkerInformationView,
		public virtual PositionView,
		public virtual RobotStatusView,
		public virtual RobotLastRequestSuccessfulView,
		public virtual TimeView {
public:
	GlobalView();
	virtual ~GlobalView();
};

#endif /* GLOBAL_VIEW_H_ */
