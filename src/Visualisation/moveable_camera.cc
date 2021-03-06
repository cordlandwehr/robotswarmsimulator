/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
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

/*
 * moveable_camera.cc
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */


#include <cmath>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"

#include "../Utilities/vector_arithmetics.h"

#include "moveable_camera.h"

const float kMouseSpeed = 4;

MoveableCamera::MoveableCamera(): Camera() {
	position_.insert_element(kXCoord, 0);
	position_.insert_element(kYCoord, 0);
	position_.insert_element(kZCoord, 0);

	view_.insert_element(kXCoord, 0);
	view_.insert_element(kYCoord, 0);
	view_.insert_element(kZCoord, 0);

	up_vector_.insert_element(kXCoord, 0);
	up_vector_.insert_element(kYCoord, 0);
	up_vector_.insert_element(kZCoord, 0);

	view_(0) = 0.0;
	view_(1) = 1.0;
	view_(2) = 0.5;

	up_vector_(1) = 1.0;
}

void MoveableCamera::set_button_press_mouse(float x, float y) {
	down_x_=x;
	down_y_=y;
}

void MoveableCamera::set_view_by_mouse(float x, float y) {
	float angle_y = 0.0f;
	float angle_z = 0.0f;

	if( (x == down_x_) && (y == down_y_) ) return;

	// Get the direction the mouse moved in.
	angle_y = (down_x_ - x ) * kMouseSpeed;
	angle_z = (down_y_ - y ) * kMouseSpeed;

	Vector3d axis = vector3d_cross(view_ - position_, up_vector_);
	vector3d_normalize(axis);

	// Rotate around our perpendicular axis and along the y-axis
	rotate_view(angle_z, axis);
	Vector3d y_axis;
	y_axis.insert_element(kXCoord, 0);
	y_axis.insert_element(kYCoord, 1.0);
	y_axis.insert_element(kZCoord, 0);

	rotate_view(angle_y, y_axis);

	down_x_=x;
	down_y_=y;
}

void MoveableCamera::strafe_camera(float speed) {
	// Add the strafe vector to our position
	position_(0) += strafe_(0) * speed;
	position_(2) += strafe_(2) * speed;

	// Add the strafe vector to our view
	view_(0) += strafe_(0) * speed;
	view_(2) += strafe_(2) * speed;
}

void MoveableCamera::move_camera_up_down(float speed) {
	// Get the current view vector (the direction we are looking)
	Vector3d n = position_ - view_;
	Vector3d u = vector3d_cross( up_vector_, n);
	Vector3d v = vector3d_cross(n, u);
	vector3d_normalize(n);
	vector3d_normalize(u);
	vector3d_normalize(v);

	position_(0) -= v(0) * speed;
	position_(1) -= v(1) * speed,
	position_(2) -= v(2) * speed;

	view_(0) -= v(0) * speed;
	view_(1) -= v(1) * speed;
	view_(2) -= v(2) * speed;
}

void MoveableCamera::move_camera(float speed) {
	// Get the current view vector (the direction we are looking)
	Vector3d vector = view_ - position_;
	vector3d_normalize(vector);

	position_(1) += vector(1) * speed;
	position_(2) += vector(2) * speed;		// Add our acceleration to our position's Z

	view_(1) += vector(1) * speed;
	view_(2) += vector(2) * speed;			// Add our acceleration to our view's Z
}

void MoveableCamera::move_forward() {
	move_camera(speed());
}

void MoveableCamera::move_backward() {
	move_camera(-speed());
}

void MoveableCamera::move_up() {
	move_camera_up_down(-speed());
}

void MoveableCamera::move_down() {
	move_camera_up_down(+speed());
}

void MoveableCamera::strafe_left() {
	strafe_camera(-speed());
}

void MoveableCamera::strafe_right() {
	strafe_camera(speed());
}

void MoveableCamera::update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
							const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
							const std::vector<boost::shared_ptr<RobotData> >& robot_data,
							double extrapolate) {
	Vector3d cross = vector3d_cross(view_ - position_, up_vector_);

	//normalize the strafe vector
	vector3d_normalize(cross);
	vector3d_set(strafe_, cross);
}

void MoveableCamera::look_rot() const {
	Vector3d n = position_ - view_;
	Vector3d u = vector3d_cross( up_vector_, n);
	Vector3d v = vector3d_cross(n, u);
	vector3d_normalize(n);
	vector3d_normalize(u);
	vector3d_normalize(v);
	float mat[16];
	mat[0] = u(0); mat[4] = u(1) ; mat[8] = u(2); mat[12] = 0;
	mat[1] = v(0); mat[5] = v(1) ; mat[9] = v(2); mat[13] = 0;
	mat[2] = n(0); mat[6] = n(1); mat[10] = n(2); mat[14] = 0;
	mat[3] = 0; mat[7] = 0; mat[11] = 0; mat[15] = 1;

	glLoadMatrixf(mat);
}

void MoveableCamera::look_translate() const {
	glTranslatef(- position_(0), - position_(1), - position_(2) );
}

std::string MoveableCamera::get_name() {
	return "Free Camera";
}
