/*
 * follow_swarm_camera.cc
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#include <cmath>

#include "../OpenGL/GLHeaders.h"
#include "../OpenGL/GLUHeaders.h"
#include "../OpenGL/GLUTHeaders.h"

#include "../Utilities/vector3d.h"

#include "../Model/world_object.h"
#include "../Model/obstacle.h"
#include "../Model/robot_data.h"

#include "follow_swarm_camera.h"

FollowSwarmCamera::FollowSwarmCamera() : Camera() {


}


void FollowSwarmCamera::update(const std::vector<boost::shared_ptr<WorldObject> > & markers,
						const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
						const std::vector<boost::shared_ptr<RobotData> >& robot_datas){

	// Set new look at and camera position
	Vector3d center = Vector3d();
	float max_width = 0;
	float max_height = 0;
	float max_depth = 0;

	float width = 0.0;
	// Return from this function if we passed in bad data

	if(markers.size() + obstacles.size() + robot_datas.size() == 0){
		width = 2.0;
	}



	// Sum up all positions to get the center point of all objects

	center = Vector3d();
	int num_objects = 0;

	std::vector<boost::shared_ptr<WorldObject> >::const_iterator it_markers;
	for(it_markers = markers.begin(); it_markers < markers.end(); it_markers++){
		center += (*it_markers)->position();
		num_objects++;

	}



	std::vector< boost::shared_ptr<RobotData> >::const_iterator it_robot_data;

	for(it_robot_data = robot_datas.begin(); it_robot_data < robot_datas.end(); it_robot_data++){
		center += (*it_robot_data)->position();
		num_objects++;
	}



	// Divide the total by the number of objects to get the center point.
	// But check first if there were objects at all.
	if(num_objects > 0){
		center /= num_objects;
	}




	// Calculate the fartest distance to the center point.
	// Calculate the distance to the center point for all objects
	for(it_markers = markers.begin(); it_markers < markers.end(); it_markers++){
		float current_width  = fabsf(center(0) - (*it_markers)->position()(0));
		float current_height = fabsf(center(1) - (*it_markers)->position()(1));
		float current_depth  = fabsf(center(2) - (*it_markers)->position()(2));



		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;


	}


	// Don't forget the robots
	for(it_robot_data = robot_datas.begin(); it_robot_data < robot_datas.end(); it_robot_data++){
		float current_width  = fabsf(center(0) - (*it_robot_data)->position()(0));
		float current_height = fabsf(center(1) - (*it_robot_data)->position()(1));
		float current_depth  = fabsf(center(2) - (*it_robot_data)->position()(2));



		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;
	}



	// Check if the width is the highest value and assign that for the cube dimension

	if(max_width > max_height && max_width > max_depth){

		width = max_width;

	// Check if the height is the heighest value and assign that for the cube dimension
	}else if(max_height > max_width && max_height > max_depth) {

		width = max_height;

	// Else it must be the depth or it's the same value as some of the other ones
	} else {

		width = max_depth;
	}


	this->view_(0) = center(0);
	view_(1) = center(1);
	view_(2) = center(2);


	//TODO: Test whether this really works
	position_(0) = center(0);
	position_(1) = center(0) + width * 2.25;
	position_(2) = center(0);


}


void FollowSwarmCamera::look() const{
	gluLookAt(position_(0), position_(1), position_(0),
				  view_(0),	 view_(1),     view_(2),
				  up_vector_(0), up_vector_(1), up_vector_(2));

};

