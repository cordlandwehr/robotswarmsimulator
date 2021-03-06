/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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
 * cog_camera.cc
 *
 *  Created on: 02.03.2009
 *      Author: kamil
 */


#include <cmath>
#include <cstdio>

#include <boost/foreach.hpp>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"

#include "../Utilities/vector_arithmetics.h"

#include "../Model/world_object.h"
#include "../Model/obstacle.h"
#include "../Model/robot_data.h"

#include "cog_camera.h"

namespace {

const double PI = 3.14159265;

const float kSpeed = 0.5f;
const float kSpeedf = 0.1;
const float kSpeedMovef = 1.0;
const float kSpeedUpf = 0.4;

const float eps = 0.4;

} // namespace

CogCamera::CogCamera(): rot_phi_(0.0), rot_theta_(1.0), radius_(5.0) {
	sphere_vec_(0)=1.0;
	sphere_vec_(1) = 1.0;
	sphere_vec_(2) = 0.0;
	calc_sphere_vec();
}

void CogCamera::strafe_camera(float speed) {
	Vector3d strafe_vec = vector3d_cross(up_vector_,sphere_vec_);
	vector3d_normalize(strafe_vec);
	sphere_vec_ = sphere_vec_ + speed * strafe_vec;
	calc_sphere_vec();
}

void CogCamera::move_camera(float speed) {
	radius_ += speed;
	if (radius_ < 1.0) {
		radius_ = 1.0;
	}

	calc_sphere_vec();
}

void CogCamera::move_forward() {
	move_camera(-kSpeedMovef);
}

void CogCamera::move_backward() {
	move_camera(kSpeedMovef);
}

void CogCamera::move_camera_up_down(float speed) {
	Vector3d strafe_vec = vector3d_cross(up_vector_,sphere_vec_);
	Vector3d up_vec = vector3d_cross(sphere_vec_, strafe_vec);
	vector3d_normalize(up_vec);

	sphere_vec_ = sphere_vec_ + speed * up_vec;

	calc_sphere_vec();
}

void CogCamera::move_up() {
	Vector3d old_sphere_vec = sphere_vec_;
	move_camera_up_down(kSpeedUpf);

	Vector3d new_sphere_vec = sphere_vec_;
	double y = std::fabs(new_sphere_vec(1)) - radius_;
	if( std::sqrt( new_sphere_vec(0)* new_sphere_vec(0) + y*y + new_sphere_vec(2)*new_sphere_vec(2) )< eps*radius_ ){
		sphere_vec_ = old_sphere_vec;
	}
}

void CogCamera::move_down() {
	Vector3d old_sphere_vec = sphere_vec_;
	move_camera_up_down( - kSpeedUpf );

	Vector3d new_sphere_vec = sphere_vec_;
	double y = std::fabs(new_sphere_vec(1)) - radius_;
	if (std::sqrt(new_sphere_vec(0) * new_sphere_vec(0) +
			      y*y +
			      new_sphere_vec(2)*new_sphere_vec(2)) < eps*radius_ ) {
		sphere_vec_ = old_sphere_vec;
	}
}

void CogCamera::strafe_left(){
	strafe_camera( -kSpeed );
}

void CogCamera::strafe_right(){
	strafe_camera( kSpeed );
}

void CogCamera::update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
		const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
		const std::vector<boost::shared_ptr<RobotData> >& robot_data,
		double extrapolate ) {
	Vector3d center;
	center.insert_element(kXCoord, 0);
	center.insert_element(kYCoord, 0);
	center.insert_element(kZCoord, 0);
	int num_objects = 0;

	BOOST_FOREACH( boost::shared_ptr<RobotData> it_robot_data, robot_data ) {
		boost::shared_ptr<Vector3d> pos = it_robot_data->extrapolated_position();

		center += (*pos);
		num_objects++;
	}

	// Divide the total by the number of objects to get the center point.
	// But check first if there were objects at all.
	if(num_objects > 0) {
		center /= num_objects;
	}

	if(need_init_pos_) {
		sphere_vec_ = init_pos_ - center;
		radius_ = boost::numeric::ublas::norm_2(sphere_vec_);
		need_init_pos_ = false;
	}

	position_ = center + sphere_vec_;
	view_ = center;
}

void CogCamera::calc_sphere_vec() {
	vector3d_normalize(sphere_vec_);

	sphere_vec_(0) = radius_ * sphere_vec_(0);
	sphere_vec_(1) = radius_ * sphere_vec_(1);
	sphere_vec_(2) = radius_ * sphere_vec_(2);

	if( sphere_vec_(0) == 0.0 && sphere_vec_(2) == 0.0 ) {
		sphere_vec_(0) = 0.1;
		sphere_vec_(2) = -0.1;
	}
}

void CogCamera::set_init_pos(Vector3d & pos) {
	need_init_pos_ = true;
	init_pos_ = pos;
}

std::string CogCamera::get_name() {
	return "Center of Gravity";
}

void CogCamera::set_rot_phi(double phi) {
	if( phi < 0) {
		rot_phi_ = 2*PI + phi;
	} else if( phi > 2*PI ) {
		rot_phi_ = phi - 2*PI;
	} else {
		rot_phi_ = phi;
	 }
}

void CogCamera::set_rot_theta(double theta) {
	if( theta < 0) {
		rot_theta_ = PI + theta;
	}else if( theta > PI ) {
		rot_theta_ = theta - PI;
	} else {
		rot_theta_ = theta;
	}
}
