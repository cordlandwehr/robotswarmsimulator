#include "robot_data.h"

RobotData::RobotData() {
	// TODO Auto-generated constructor stub

}

RobotData::~RobotData() {
	// TODO Auto-generated destructor stub
}

const Vector3d & RobotData::acceleration() const {
	return *acceleration_;
}

void RobotData::set_acceleration(boost::shared_ptr<Vector3d> new_acceleration) {
	acceleration_ = new_acceleration;
}

boost::tuple<boost::shared_ptr<Vector3d> > RobotData::coordinate_system_axis() const {
	return coordinate_system_axis_;
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

RobotStatus RobotData::status() const {
	return status_;
}

void RobotData::set_status(RobotStatus new_status) {
	status_ = new_status;
}
