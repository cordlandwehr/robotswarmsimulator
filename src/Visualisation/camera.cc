/*
 * camera.cc
 *
 *  Created on: 15.01.2009
 *      Author: kamil
 */
#include <cmath>

//#ifdef WIN32
//	#include <GL/gl.h>
//	#include <GL/glu.h>
//	#include <GL/glut.h>
//#else
//	#include <gl.h>
//	#include <glu.h>
//	#include <glut.h>
//#endif
#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/glut_headers.h"

#include "../Utilities/vector_arithmetics.h"

#include "camera.h"




Vector3d  Cross(const Vector3d & vector1 ,const Vector3d & vector2) {

	Vector3d normal = Vector3d();

	// Calculate the cross product with the non communitive equation


	normal(0) = vector1(1) * vector2(2) - vector1(2) * vector2(1);
	normal(1) = vector1(2) * vector2(0) - vector1(0) * vector2(2);
	normal(2) = vector1(0) * vector2(1) - vector1(1) * vector2(0);

	// Return the cross product

	return normal;

}


float Magnitude(const Vector3d & vec) {

	return (float)std::sqrt( vec(0) * vec(0) + vec(1) * vec(1) + vec(2) * vec(2) );

}


Vector3d & Normalize( Vector3d & vector) {

	// Get the magnitude of our normal

	float magnitude = Magnitude(vector);

	vector = vector / magnitude;



	return vector;

}


Camera::Camera() {

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



Camera::~Camera(){

}

void Camera::position_camera(const Vector3d & position,const Vector3d &view, const Vector3d &up_vector) {

	position_	= position;
	view_ = view;
	up_vector_ = up_vector;
}



void Camera::set_view_by_mouse(int x, int y) {

	// No Operation

}

void Camera::set_button_press_mouse(int x, int y) {

	// No Operation

}





void Camera::rotate_view(float angle, const Vector3d & rot_vec) {

	Vector3d new_view;
	Vector3d view;


	// Get the view vector (The direction we are facing)

	view = view_ - position_;


	// Calculate the sine and cosine of the angle once

	float cosTheta = (float)std::cos(angle);
	float sinTheta = (float)std::sin(angle);


	// Find the new x position for the new rotated point

	new_view(0)  = (cosTheta + (1 - cosTheta)*rot_vec(0)*rot_vec(0) )               * view(0);
	new_view(0) += ((1 - cosTheta) * rot_vec(0) * rot_vec(1) - rot_vec(2) * sinTheta)	* view(1);
	new_view(0) += ((1 - cosTheta) * rot_vec(0) * rot_vec(2) + rot_vec(1) * sinTheta)	* view(2);



	// Find the new y position for the new rotated point

	new_view(1)  = ((1 - cosTheta) * rot_vec(0) * rot_vec(1) + rot_vec(2) * sinTheta)	* view(0);
	new_view(1) += (cosTheta + (1 - cosTheta) * rot_vec(1) * rot_vec(1) )               * view(1);
	new_view(1) += ((1 - cosTheta) * rot_vec(1) * rot_vec(2) - rot_vec(0) * sinTheta)	* view(2);

	// Find the new z position for the new rotated point

	new_view(2)  = ((1 - cosTheta) * rot_vec(0) * rot_vec(2) - rot_vec(1) * sinTheta)	* view(0);
	new_view(2) += ((1 - cosTheta) * rot_vec(1) * rot_vec(2) + rot_vec(0) * sinTheta)	* view(1);
	new_view(2) += (cosTheta + (1 - cosTheta) * rot_vec(2) * rot_vec(2) )		        * view(2);

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.

	view_ = position_ + new_view;
	}




void Camera::strafe_camera(float speed) {

}



void Camera::move_camera(float speed) {

}


void Camera::move_forward(){

}

void Camera::move_backward(){

}

void Camera::move_up(){

}


void Camera::move_down(){

}


void Camera::rotate_around_point(Vector3d center, Vector3d rot_vec) {
}

void Camera::strafe_left(){

}

void Camera::strafe_right(){

}




void Camera::update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
		const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
		const std::vector<boost::shared_ptr<RobotData> >& robot_data,
		double extrapolate ) {


 // No Operation

}



void Camera::look() const{

	look_rot();
	look_translate();

}

void Camera::look_rot() const{

	// Works like the gluLookAt function works
	Vector3d n = position_ - view_;
	Vector3d u = Cross( up_vector_, n);
	Vector3d v = Cross(n, u);
	n = Normalize(n);
	u = Normalize(u);
	v = Normalize(v);

	// create matrix
	float mat[16];
	mat[0] = u(0); mat[4] = u(1) ; mat[8] = u(2); mat[12] = 0;
	mat[1] = v(0); mat[5] = v(1) ; mat[9] = v(2); mat[13] = 0;
	mat[2] = n(0); mat[6] = n(1); mat[10] = n(2); mat[14] = 0;
	mat[3] = 0; mat[7] = 0; mat[11] = 0; mat[15] = 1;

	glLoadMatrixf(mat);

}

void Camera::look_translate() const{
	// Do translaten
	glTranslatef(- position_(0), - position_(1), - position_(2) );
}

std::string Camera::get_name(){

	return "Base Camera";
}

