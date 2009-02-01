/*
 * moveable_camera.cc
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */


#include <cmath>

#include "../OpenGL/GLHeaders.h"
#include "../OpenGL/GLUHeaders.h"
#include "../OpenGL/GLUTHeaders.h"

#include "../Utilities/vector3d.h"

#include "moveable_camera.h"

MoveableCamera::MoveableCamera(): Camera() {

	position_ = Vector3d();

	view_ = Vector3d();

	up_vector_   = Vector3d();


	view_(0) = 0.0;
	view_(1) = 1.0;
	view_(2) = 0.5;

	up_vector_(1) = 1.0;

}



void MoveableCamera::set_view_by_mouse(int x, int y) {


	int middle_x = screen_width_  >> 1;

	int middle_y = screen_height_ >> 1;

	float angle_y = 0.0f;							// This is the direction for looking up or down

	float angle_z = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)

	static float current_rot_x = 0.0f;





	// If our cursor is still in the middle, we never moved... so don't update the screen

	if( (x == middle_x) && (y == middle_y) ) return;



	// Set the mouse position to the middle of our window

	glutWarpPointer(middle_x, middle_y);



	// Get the direction the mouse moved in, but bring the number down to a reasonable amount

	angle_y = (float)( (middle_x - x ) ) / 1000.0f;

	angle_z = (float)( (middle_y - y ) ) / 1000.0f;



	// Here we keep track of the current rotation (for up and down) so that

	// we can restrict the camera from doing a full 360 loop.

	current_rot_x -= angle_z;



	// If the current rotation (in radians) is greater than 1.0, we want to cap it.

	if(current_rot_x > 1.0f){

		current_rot_x = 1.0f;

	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue

	} else if(current_rot_x < -1.0f) {

		current_rot_x = -1.0f;

	// Otherwise, we can rotate the view around our position

	} else {

		// To find the axis we need to rotate around for up and down

		// movements, we need to get a perpendicular vector from the

		// camera's view vector and up vector.  This will be the axis.

		Vector3d axis = Cross(view_ - position_, up_vector_);

		axis = Normalize(axis);



		// Rotate around our perpendicular axis and along the y-axis

		rotate_view(angle_z, axis);
		Vector3d y_axis = Vector3d();
		y_axis(1) = 1.0;

		rotate_view(angle_y, y_axis);

	}

}








void MoveableCamera::strafe_camera(float speed) {



	// Add the strafe vector to our position

	position_(0) += strafe_(0) * speed;
	//position_(1) += strafe_(1) * speed;
	position_(2) += strafe_(2) * speed;



	// Add the strafe vector to our view

	view_(0) += strafe_(0) * speed;
	//view_(1) += strafe_(1) * speed;
	view_(2) += strafe_(2) * speed;

}




void MoveableCamera::move_camera(float speed) {

	// Get the current view vector (the direction we are looking)

	Vector3d vector = view_ - position_;

	vector = Normalize(vector);



	position_(0) += vector(0) * speed;		// Add our acceleration to our position's X
	//position_(1) += vector(1) * speed;
	position_(2) += vector(2) * speed;		// Add our acceleration to our position's Z

	view_(0) += vector(0) * speed;			// Add our acceleration to our view's X
	//view_(1) += vector(1) * speed;
	view_(2) += vector(2) * speed;			// Add our acceleration to our view's Z

}



void MoveableCamera::move_forward(){
	move_camera(kSpeed);
}

void MoveableCamera::move_backward(){
	move_camera(-kSpeed);
}

void MoveableCamera::strafe_left(){
	strafe_camera(-kSpeed);
}

void MoveableCamera::strafe_right(){
	strafe_camera(kSpeed);
}




void MoveableCamera::update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
							const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
							const std::vector<boost::shared_ptr<RobotData> >& robot_data) {



	Vector3d cross = Cross(view_ - position_, up_vector_);



	// Normalize the strafe vector

	strafe_ = Normalize(cross);


}



void MoveableCamera::look() const{

	// Give openGL our camera position, then camera view, then camera up vector

	gluLookAt(position_(0), position_(1), position_(0),
			  view_(0),	 view_(1),     view_(2),
			  up_vector_(0), up_vector_(1), up_vector_(2));

}
