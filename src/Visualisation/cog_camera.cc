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
#include "../OpenGL/glut_headers.h"

#include "../Utilities/vector_arithmetics.h"

#include "../Model/world_object.h"
#include "../Model/obstacle.h"
#include "../Model/robot_data.h"

#include "cog_camera.h"

namespace {

const double PI = 3.14159265;

const float kSpeedf = 0.1;
const float kSpeedMovef = 2.0;

}
CogCamera::CogCamera(): rot_theta_(1.0), rot_phi_(0.0), radius_(5.0){

	calc_sphere_vec();
}



void CogCamera::strafe_camera(float speed){


	set_rot_theta( rot_theta_ + speed );
	calc_sphere_vec();



}


 void CogCamera::move_camera(float speed){

	 radius_ += speed;
	 if( radius_ < 1.0){
		 radius_ = 1.0;
	 }

	 calc_sphere_vec();

 }

 void CogCamera::move_forward(){

	 move_camera(- kSpeedMovef );
 }

 void CogCamera::move_backward(){

	 move_camera( kSpeedMovef );
 }

 void CogCamera::move_camera_up_down(float speed){
	 set_rot_phi( rot_phi_ + speed );
	 calc_sphere_vec();
 }

 void CogCamera::move_up(){
	 move_camera_up_down(kSpeedf);

 }

void CogCamera::move_down(){
	move_camera_up_down( - kSpeedf );
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
		double extrapolate ){



	Vector3d center;
	center.insert_element(kXCoord, 0);
	center.insert_element(kYCoord, 0);
	center.insert_element(kZCoord, 0);
	int num_objects = 0;



	BOOST_FOREACH( boost::shared_ptr<RobotData> it_robot_data, robot_data ){
		boost::shared_ptr<Vector3d> pos = it_robot_data->extrapolated_position(extrapolate);

		center += (*pos);
		num_objects++;
	}


	// Divide the total by the number of objects to get the center point.
	// But check first if there were objects at all.
	if(num_objects > 0){
		center /= num_objects;
	}



	position_ = center + sphere_vec_;
	view_ = center;


}

void CogCamera::calc_sphere_vec(){

	sphere_vec_(0) = radius_ * std::sin( rot_theta_ )* std::cos( rot_phi_   );
	sphere_vec_(1) = radius_ * std::cos( rot_theta_ ) * std::sin( rot_phi_   );
	sphere_vec_(2) = radius_ * std::cos( rot_theta_  );
}



std::string CogCamera::get_name(){

	return "Center of Gravity";
}

void CogCamera::set_rot_phi(double phi){
	if( phi < 0) {
		rot_phi_ = 2*PI + phi;
	} else if( phi > 2*PI ) {
		rot_phi_ = phi - 2*PI;
	} else {
		rot_phi_ = phi;
	 }
}

void CogCamera::set_rot_theta(double theta){
	if( theta < 0){
		rot_theta_ = PI + theta;
	}else if( theta > PI ) {
		rot_theta_ = theta - PI;
	} else {
		rot_theta_ = theta;
	}
}

