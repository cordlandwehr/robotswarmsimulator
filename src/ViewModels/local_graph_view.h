/*
 * local_graph_view.h
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#ifndef LOCAL_GRAPH_VIEW_H_
#define LOCAL_GRAPH_VIEW_H_

#include "../Views/coordinate_system_view.h"
#include "../Views/id_view.h"
#include "../Views/marker_information_view.h"
#include "../Views/position_view.h"
#include "../Views/robot_status_view.h"
#include "../Views/robot_last_request_successful_view.h"
#include "../Views/robot_type_view.h"
#include "../Views/origin_view.h"
#include "../Views/time_view.h"
#include "../Views/edge_view.h"

/**
 * \class LocalGraphView
 * \brief View model for local graph algorithms.
 *
 * Assigning this class to a Robot corresponds to a "see adjacent nodes" model.
 *
 */

class LocalGraphView: public virtual CoordinateSystemView,
		public virtual EdgeView,
		public virtual IdView,
		public virtual MarkerInformationView,
		public virtual PositionView,
		public virtual RobotStatusView,
		public virtual RobotLastRequestSuccessfulView,
		public virtual RobotTypeView,
		public virtual OriginView,
		public virtual TimeView {
public:
	LocalGraphView();
	virtual ~LocalGraphView();
};

#endif /* LOCAL_GRAPH_VIEW_H_ */
