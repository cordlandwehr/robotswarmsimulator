/*
 * follow_swarm_camera.cc
 *
 *  Created on: 18.01.2009
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

#include "follow_swarm_camera.h"

FollowSwarmCamera::FollowSwarmCamera() : Camera() {}

void FollowSwarmCamera::update(const std::vector<boost::shared_ptr<WorldObject> > & markers,
						const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
						const std::vector<boost::shared_ptr<RobotData> >& robot_datas,
						double extrapolate ){
	// Set new look at and camera position
	extrapolate = extrapolate + 0.02;

	// Sum up all positions to get the center point of all objects
	Vector3d center;
	center.insert_element(kXCoord, 0);
	center.insert_element(kYCoord, 0);
	center.insert_element(kZCoord, 0);
	int num_objects = markers.size() + robot_datas.size();

	std::vector<boost::shared_ptr<WorldObject> >::const_iterator it_markers;
	for(it_markers = markers.begin(); it_markers < markers.end(); it_markers++){
		center += (*it_markers)->position();
	}

	std::vector< boost::shared_ptr<RobotData> >::const_iterator it_robot_data;
	for(it_robot_data = robot_datas.begin(); it_robot_data < robot_datas.end(); it_robot_data++){
		boost::shared_ptr<Vector3d> pos = (*it_robot_data)->extrapolated_position(extrapolate);
		center += (*pos);
	}

	// Divide the total by the number of objects to get the center point.
	// But check first if there were objects at all.
	if(num_objects > 0){
		center /= num_objects;
	}

	// Calculate the fartest distance to the center point.
	float max_width = 0;
	float max_height = 0;
	float max_depth = 0;

	// Calculate the distance to the center point for all robots and markers
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
		float current_width  = fabsf(center(0) - (*(*it_robot_data)->extrapolated_position(extrapolate))(0));
		float current_height = fabsf(center(1) - (*(*it_robot_data)->extrapolated_position(extrapolate))(1));
		float current_depth  = fabsf(center(2) - (*(*it_robot_data)->extrapolated_position(extrapolate))(2));

		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;
	}

	if( max_width < 1.0)
		max_width = 1.0;

	if( max_height < 1.0 )
		max_height = 1.0;

	if(max_depth < 1.0 ){
		max_depth = 1.0;
	}

	this->view_(0) = center(0);
	view_(1) = center(1);
	view_(2) = center(2);

	position_(0) = center(0)  + max_width * 1.3;
	position_(1) = center(1) + max_height *1.4 ;
	position_(2) = center(2) + max_depth* 1.4;
}

void FollowSwarmCamera::look_rot() const {

	Vector3d n = position_ - view_;
	Vector3d u = Cross( up_vector_, n);
	Vector3d v = Cross(n, u);
	n = Normalize(n);
	u = Normalize(u);
	v = Normalize(v);
	float mat[16];
	mat[0] = u(0); mat[4] = u(1) ; mat[8] = u(2); mat[12] = 0;
	mat[1] = v(0); mat[5] = v(1) ; mat[9] = v(2); mat[13] = 0;
	mat[2] = n(0); mat[6] = n(1); mat[10] = n(2); mat[14] = 0;
	mat[3] = 0; mat[7] = 0; mat[11] = 0; mat[15] = 1;

	glLoadMatrixf(mat);
}

void FollowSwarmCamera::look_translate() const {
	glTranslatef(- position_(0), - position_(1), - position_(2) );
}

std::string FollowSwarmCamera::get_name() {
	return "Follow Swarm Camera";
}
