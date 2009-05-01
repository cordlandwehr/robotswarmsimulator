/*
 * self_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef SELF_VIEW_H_
#define SELF_VIEW_H_

#include "../Views/own_acceleration_view.h"
#include "../Views/own_coordinate_system_view.h"
#include "../Views/own_id_view.h"
#include "../Views/own_marker_view.h"
#include "../Views/own_position_view.h"
#include "../Views/own_status_view.h"
#include "../Views/own_last_request_successful_view.h"
#include "../Views/own_type_view.h"
#include "../Views/own_velocity_view.h"

/**
 * \class SelfView
 * \brief "Known everything about yourself, but nothing about others" view model.
 *
 * Assigning this class to a Robot corresponds to a "known everything about yourself, but nothing about others" model.
 *
 */

class SelfView: public virtual OwnAccelerationView,
		public virtual OwnCoordinateSystemView,
		public virtual OwnIdView,
		public virtual OwnMarkerView,
		public virtual OwnPositionView,
		public virtual OwnStatusView,
		public virtual OwnLastRequestSuccessfulView,
		public virtual OwnTypeView,
		public virtual OwnVelocityView {
public:
	SelfView();
	virtual ~SelfView();
};

#endif /* SELF_VIEW_H_ */
