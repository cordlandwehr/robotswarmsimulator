/*
 * global_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef GLOBAL_VIEW_H_
#define GLOBAL_VIEW_H_

#include "box_view.h"
#include "acceleration_view.h"
#include "coordinate_system_view.h"
#include "full_view.h"
#include "id_view.h"
#include "marker_information_view.h"
#include "point_in_obstacle_view.h"
#include "position_view.h"
#include "robot_status_view.h"
#include "robot_type_view.h"
#include "sphere_view.h"
#include "velocity_view.h"

class GlobalView: public virtual BoxView,
		public virtual AccelerationView,
		public virtual CoordinateSystemView,
		public virtual FullView,
		public virtual IdView,
		public virtual MarkerInformationView,
		public virtual PointInObstacleView,
		public virtual PositionView,
		public virtual RobotStatusView,
		public virtual RobotTypeView,
		public virtual SphereView,
		public virtual VelocityView {
public:
	GlobalView();
	virtual ~GlobalView();
};

#endif /* GLOBAL_VIEW_H_ */
