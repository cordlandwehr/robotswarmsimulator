/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

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

#include "robot_data.h"
#include "robot.h"
#include <math.h>
#include <boost/foreach.hpp>
#include "../Views/view.h"

class View;

RobotData::RobotData(boost::shared_ptr<Identifier> id,
                     boost::shared_ptr<Vector3d> position, const Robot& robot)
                     : WorldObject(id, position), robot_(robot), last_request_successful_(true), color_(0)
{
	;
}

RobotData::RobotData(boost::shared_ptr<Identifier> id,
                     boost::shared_ptr<Vector3d> position,
                     boost::shared_ptr<MarkerInformation> marker_information, const Robot& robot)
                     : WorldObject(id, position, marker_information), robot_(robot),last_request_successful_(true), color_(0)
{
	;
}

RobotData::RobotData(const RobotData& rhs) : WorldObject(rhs), robot_(rhs.robot_), type_(rhs.type_), 
                                             status_(rhs.status_),
                                             last_request_successful_(rhs.last_request_successful_),
                                             view_(rhs.view_),
                                             color_(rhs.color_),
                                             edges_(rhs.edges_),
                                             messages_(rhs.messages_){
	if(rhs.acceleration_) {
		this->acceleration_.reset(new Vector3d(*rhs.acceleration_));
	}
	if(rhs.coordinate_system_axis_.get<0>()) {
		this->coordinate_system_axis_.get<0>().reset(new Vector3d(*rhs.coordinate_system_axis_.get<0>()));
	}
	if(rhs.coordinate_system_axis_.get<1>()) {
		this->coordinate_system_axis_.get<1>().reset(new Vector3d(*rhs.coordinate_system_axis_.get<1>()));
	}
	if(rhs.coordinate_system_axis_.get<2>()) {
		this->coordinate_system_axis_.get<2>().reset(new Vector3d(*rhs.coordinate_system_axis_.get<2>()));
	}
	if(rhs.velocity_) {
		this->velocity_.reset(new Vector3d(*rhs.velocity_));
	}
}

RobotData::~RobotData() {

}

const Vector3d & RobotData::acceleration() const {
	return *acceleration_;
}

void RobotData::set_acceleration(boost::shared_ptr<Vector3d> new_acceleration) {
	acceleration_ = new_acceleration;
}

const boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> >&
	RobotData::coordinate_system_axis() const {
	return coordinate_system_axis_;
}

void RobotData::set_coordinate_system_axis(boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
		boost::shared_ptr<Vector3d> > new_axes) {
	coordinate_system_axis_ = new_axes;
}

const RobotType RobotData::type() const {
	return type_;
}

const Vector3d & RobotData::velocity() const {
	return *velocity_;
}

void RobotData::set_velocity(boost::shared_ptr<Vector3d> new_velocity) {
	velocity_ = new_velocity;
}

boost::shared_ptr<Vector3d> RobotData::extrapolated_position() const {
	boost::shared_ptr<Vector3d> next_position(new Vector3d());
	// the next position is simply the old position (previously, this function took acceleration and timesteps into account)
	next_position->insert_element(kXCoord,
	                              position()(0));
	next_position->insert_element(kYCoord,
	                              position()(1));
	next_position->insert_element(kZCoord,
	                              position()(2));
	return next_position;
}

boost::shared_ptr<Vector3d> RobotData::extrapolated_velocity(double timesteps) const {
	boost::shared_ptr<Vector3d> next_velocity(new Vector3d());
	next_velocity->insert_element(kXCoord, velocity()(0) + timesteps * acceleration()(0));
	next_velocity->insert_element(kYCoord, velocity()(1) + timesteps * acceleration()(1));
	next_velocity->insert_element(kZCoord, velocity()(2) + timesteps * acceleration()(2));
	return next_velocity;
}

RobotStatus RobotData::status() const {
	return status_;
}

void RobotData::set_status(RobotStatus new_status) {
	status_ = new_status;
}

void RobotData::set_view(boost::weak_ptr<View> view) {
	view_ = view;
}

boost::weak_ptr<const View> RobotData::view()  {
	return view_;
}

const Robot& RobotData::robot() const {
	return robot_;
}

boost::shared_ptr<WorldObject> RobotData::clone() const {
	return boost::shared_ptr<WorldObject>(new RobotData(*this));
}

const std::vector<boost::shared_ptr<EdgeIdentifier> >& RobotData::get_outgoing_edges() const{
	assert(view_.expired() == false);
	return view_.lock()->get_visible_edges(this->robot());
}
