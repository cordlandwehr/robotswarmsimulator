/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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

/**
 * \file radial_view.h
 * \author Peter Kling
 * \date 14.05.09
 *  
 * Copyright 2009 Peter Kling. All rights reserved.
 */

#ifndef RADIAL_VIEW_H
#define RADIAL_VIEW_H

#include "../Views/box_view.h"
#include "../Views/acceleration_view.h"
#include "../Views/coordinate_system_view.h"
#include "../Views/id_view.h"
#include "../Views/marker_information_view.h"
#include "../Views/point_in_obstacle_view.h"
#include "../Views/position_view.h"
#include "../Views/robot_status_view.h"
#include "../Views/robot_last_request_successful_view.h"
#include "../Views/robot_type_view.h"
#include "../Views/sphere_view.h"
#include "../Views/spheric_view.h"
#include "../Views/velocity_view.h"
#include "../Views/origin_view.h"
#include "../Views/time_view.h"


/**
 * \brief A view model where robot's have a limited view radius, but can see any information of objects in this radius.
 */
class RadialView :
	public virtual SphericView,
	public virtual AccelerationView,
	public virtual CoordinateSystemView,
	public virtual IdView,
	public virtual MarkerInformationView,
	public virtual PointInObstacleView,
	public virtual PositionView,
	public virtual RobotStatusView,
	public virtual RobotLastRequestSuccessfulView,
	public virtual RobotTypeView,
	public virtual SphereView,
	public virtual OriginView,
	public virtual TimeView,
	public virtual VelocityView {
public:
	RadialView(double view_radius);
	virtual ~RadialView();
};


#endif /* RADIAL_VIEW_H */
