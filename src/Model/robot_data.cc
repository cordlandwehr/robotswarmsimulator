#include "robot_data.h"

RobotData::RobotData() {
	// TODO Auto-generated constructor stub

}

RobotData::~RobotData() {
	// TODO Auto-generated destructor stub
}

const boost::shared_ptr<Identifier>& RobotData::id() const {
	return id_;
}

std::vector<double> RobotData::acceleration() const {
	return acceleration_;
}

void RobotData::set_acceleration(std::vector<double> new_acceleration) {
	acceleration_ = new_acceleration;
}

const boost::tuple<std::vector<double> > RobotData::coordinate_system_axis() const {
	return coordinate_system_axis_;
}

const Type RobotData::type() const {
	return type_;
}

std::vector<double> RobotData::velocity() const {
	return velocity_;
}

void RobotData::set_velocity(std::vector<double> new_velocity) {
	velocity_ = new_velocity;
}

Status RobotData::status() const {
	return status_;
}

void RobotData::set_status(Status new_status) {
	status_ = new_status;
}
