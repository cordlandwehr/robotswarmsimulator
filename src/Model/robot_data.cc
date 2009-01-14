#include "robot_data.h"
#include "robot.h"

RobotData::RobotData(boost::shared_ptr<Identifier> id,
                     boost::shared_ptr<Vector3d> position, const Robot& robot)
                     : WorldObject(id, position), robot_(robot)
{
	;
}

RobotData::RobotData(boost::shared_ptr<Identifier> id,
                     boost::shared_ptr<Vector3d> position,
                     boost::shared_ptr<MarkerInformation> marker_information, const Robot& robot)
                     : WorldObject(id, position, marker_information), robot_(robot)
{
	;
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

boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> >
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

Vector3d RobotData::extrapolated_position(int timesteps) const {
	Vector3d next_position; // TODO cola: maybe the velocity should be increased by acceleration
	next_position(0) = position()(0) + timesteps * velocity()(0);
	next_position(1) = position()(1) + timesteps * velocity()(1);
	next_position(2) = position()(2) + timesteps * velocity()(2);
	return next_position;
}

Vector3d RobotData::extrapolated_velocity(int timesteps) const {
	Vector3d next_velocity;
	next_velocity(0) = velocity()(0) + timesteps * acceleration()(0);
	next_velocity(1) = velocity()(1) + timesteps * acceleration()(1);
	next_velocity(2) = velocity()(2) + timesteps * acceleration()(2);
	return next_velocity;
}

RobotStatus RobotData::status() const {
	return status_;
}

void RobotData::set_status(RobotStatus new_status) {
	status_ = new_status;
}

const Robot& RobotData::robot() const {
	return robot_;
}
