/*
 * orthogonal_camera.cpp
 *
 *  Created on: 09.09.2011
 *      Author: Sascha Brandt
 */

#include <cmath>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"

#include "../Utilities/vector_arithmetics.h"

#include "orthogonal_camera.h"

OrthogonalCamera::OrthogonalCamera(): Camera(), zoom_(0.1), ortho_axis_(Z_AXIS) {
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
	view_(1) = 0.0;
	view_(2) = -1.0;

	position_(0) = 0.5;
	position_(1) = -0.5;
	position_(2) = -1;

	up_vector_(0) = 0.0;
	up_vector_(1) = 1.0;
	up_vector_(2) = 0.0;
}

OrthogonalCamera::~OrthogonalCamera() {
}

void OrthogonalCamera::set_button_press_mouse(float x, float y) {
	down_x_=x;
	down_y_=y;
}

void OrthogonalCamera::set_view_by_mouse(float x, float y) {
	float move_x = 0.0f;
	float move_y = 0.0f;

	if( (x == down_x_) && (y == down_y_) ) return;

	// Get the direction the mouse moved in, but bring the number down to a reasonable amount.
	move_x = down_x_ - x;
	move_y = down_y_ - y;

	move_camera(-move_y);
	strafe_camera(move_x);

	down_x_=x;
	down_y_=y;
}

void OrthogonalCamera::strafe_camera(float speed) {
	switch(ortho_axis_) {
	case X_AXIS:
		position_(2) += speed;
		break;
	case Y_AXIS:
		position_(0) -= speed;
		break;
	case Z_AXIS:
		position_(0) -= speed;
		break;
	}
}

void OrthogonalCamera::move_camera_up_down(float speed) {
	zoom_ *= speed < 0 ? 1.0-speed : 1.0/(1.0+speed);
}

void OrthogonalCamera::move_camera(float speed) {
	switch(ortho_axis_) {
	case X_AXIS:
		position_(1) += speed;
		break;
	case Y_AXIS:
		position_(2) -= speed;
		break;
	case Z_AXIS:
		position_(1) += speed;
		break;
	}
}

void OrthogonalCamera::move_forward() {
	move_camera(zoom_*speed());
}

void OrthogonalCamera::move_backward() {
	move_camera(-zoom_*speed());
}

void OrthogonalCamera::move_up() {
	move_camera_up_down(-speed());
}

void OrthogonalCamera::move_down() {
	move_camera_up_down(+speed());
}

void OrthogonalCamera::strafe_left() {
	strafe_camera(-zoom_*speed());
}

void OrthogonalCamera::strafe_right() {
	strafe_camera(zoom_*speed());
}

void OrthogonalCamera::update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
							const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
							const std::vector<boost::shared_ptr<RobotData> >& robot_data,
							double extrapolate) {
	// use zoom factor as position for the orthogonal axis, but clamp to frustrum bounds
	position_(ortho_axis_) = -std::max(std::min(1000.0f, 1.0f/zoom_), 0.1f);
}

void OrthogonalCamera::look_rot() const {
	Vector3d n = view_;
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

void OrthogonalCamera::look_translate() const {
	glTranslatef(- position_(0), - position_(1),  - position_(2) );
	glScalef( zoom_, zoom_, zoom_ );
}

std::string OrthogonalCamera::get_name() {
	return "Orthogonal Camera";
}


void OrthogonalCamera::set_ortho_axis(int axis) {
	ortho_axis_ = axis;
	switch(axis) {
	case X_AXIS:
		view_(0) = -1.0;
		view_(1) = 0.0;
		view_(2) = 0.0;

		position_(0) = -1;
		position_(1) = -0.5;
		position_(2) = -0.5;

		up_vector_(0) = 0.0;
		up_vector_(1) = 1.0;
		up_vector_(2) = 0.0;

		zoom_ = 0.1;
		break;
	case Y_AXIS:
		view_(0) = 0.0;
		view_(1) = -1.0;
		view_(2) = 0.0;

		position_(0) = 0.5;
		position_(1) = -1;
		position_(2) = 0.5;

		up_vector_(0) = 0.0;
		up_vector_(1) = 0.0;
		up_vector_(2) = -1.0;

		zoom_ = 0.1;
		break;
	case Z_AXIS:
		view_(0) = 0.0;
		view_(1) = 0.0;
		view_(2) = -1.0;

		position_(0) = 0.5;
		position_(1) = -0.5;
		position_(2) = -1;

		up_vector_(0) = 0.0;
		up_vector_(1) = 1.0;
		up_vector_(2) = 0.0;

		zoom_ = 0.1;
		break;
	}
}
