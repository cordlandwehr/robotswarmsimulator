/*
 * global_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef GLOBAL_VIEW_H_
#define GLOBAL_VIEW_H_

#include "../Views/box_view.h"
#include "../Views/acceleration_view.h"
#include "../Views/coordinate_system_view.h"
#include "../Views/full_view.h"
#include "../Views/id_view.h"
#include "../Views/marker_information_view.h"
#include "../Views/point_in_obstacle_view.h"
#include "../Views/position_view.h"
#include "../Views/robot_status_view.h"
#include "../Views/robot_type_view.h"
#include "../Views/sphere_view.h"
#include "../Views/velocity_view.h"
#include "../Views/origin_view.h"
#include "../Views/time_view.h"

/**
 * \class GlobalView
 * \brief Global information view model.
 *
 * Assigning this class to a Robot corresponds to a "see everything, everywhere" model.
 *
 */

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
		public virtual OriginView,
		public virtual TimeView,
		public virtual VelocityView {
public:
	GlobalView();
	virtual ~GlobalView();
};

#endif /* GLOBAL_VIEW_H_ */
