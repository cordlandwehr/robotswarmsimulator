/*
 * octree.cc
 *
 *  Created on: 08.01.2009
 *      Author: kamil
 */

#include <cmath>
#include <boost/foreach.hpp>

#include "../Model/obstacle.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/marker_information.h"
#include "../Model/sphere.h"
#include "../Model/box.h"

#include "octree.h"




Octree::Octree() {


	max_levels_ = 5;
	min_width_ = 2.0;



	init_octree();

}

Octree::Octree(int max_levels): max_levels_(max_levels), min_width_(1.0) {

	init_octree();
}

Octree::Octree(double min_width): max_levels_(5), min_width_(min_width) {

	init_octree();
}

Octree::Octree(int max_levels, double min_width): max_levels_(max_levels), min_width_(min_width) {



	init_octree();

}



Octree::~Octree() {



}

void Octree::init_octree(){


}




void Octree::OctreeNode::init_node() {


	sub_divided_ = false;
	width_ = 0.0;
	object_count_ = 0;

	center_ = Vector3d();

	center_.insert_element(kXCoord, 0);
	center_.insert_element(kYCoord, 0);
	center_.insert_element(kZCoord, 0);


}


void Octree::OctreeNode::scene_dimensions(const std::vector< boost::shared_ptr<WorldObject> > & markers,
							  const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
							  const std::vector< boost::shared_ptr<RobotData> > & robot_datas) {


	double max_width = 0;
	double max_height = 0;
	double max_depth = 0;


	if(markers.size() + obstacles.size() + robot_datas.size() == 0){
		width_ = 0.0;
	}



	// Sum up all positions to get the center point of all objects

	center_ = Vector3d();
	center_.insert_element(kXCoord, 0);
	center_.insert_element(kYCoord, 0);
	center_.insert_element(kZCoord, 0);

	int num_objects = 0;
	BOOST_FOREACH( boost::shared_ptr<WorldObject>  it_markers, markers){
	//std::vector<boost::shared_ptr<WorldObject> >::const_iterator it_markers;
	//for(it_markers = markers.begin(); it_markers != markers.end(); it_markers++){
		center_+= it_markers->position();
		num_objects++;

	}


	BOOST_FOREACH( boost::shared_ptr<Obstacle> it_obstacles, obstacles){
	//std::vector< boost::shared_ptr<Obstacle> >::const_iterator it_obstacles;
	//for(it_obstacles = obstacles.begin(); it_obstacles != obstacles.end(); it_obstacles++){
		center_ +=  it_obstacles->position();
		num_objects++;
	}

	//std::vector< boost::shared_ptr<RobotData> >::const_iterator it_robot_data;
	BOOST_FOREACH( boost::shared_ptr<RobotData> it_robot_data , robot_datas){
	//for(it_robot_data = robot_datas.begin(); it_robot_data != robot_datas.end(); it_robot_data++){
		center_ += it_robot_data->position();
		num_objects++;
	}





	// Divide the total by the number of objects to get the center point.
	// But check first if there were objects at all.
	if(num_objects > 0){
		center_ /= num_objects;
	}




	// Calculate the farest distance to the center point.
	// Calculate the distance to the center point for all objects
	BOOST_FOREACH( boost::shared_ptr<WorldObject> it_markers, markers ){
	//for(it_markers = markers.begin(); it_markers < markers.end(); it_markers++){
		double current_width  = fabsf(center_(0) - it_markers->position()(0));
		double current_height = fabsf(center_(1) - it_markers->position()(1));
		double current_depth  = fabsf(center_(2) - it_markers->position()(2));



		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;


	}

	BOOST_FOREACH( boost::shared_ptr<Obstacle> it_obstacles, obstacles ){
	//for(it_obstacles = obstacles.begin(); it_obstacles < obstacles.end(); it_obstacles++){
		// Add the max size of he obstacle to width, height and depth.
		double obstacle_max_size = it_obstacles->max_dimension();

		double current_width  = fabsf(center_(0) - it_obstacles->position()(0)) + obstacle_max_size;
		double current_height = fabsf(center_(1) - it_obstacles->position()(1)) + obstacle_max_size;
		double current_depth  = fabsf(center_(2) - it_obstacles->position()(2)) + obstacle_max_size;



		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;


	}

	// Don't forget the robots
	BOOST_FOREACH( boost::shared_ptr<RobotData> it_robot_data, robot_datas ){
	//for(it_robot_data = robot_datas.begin(); it_robot_data < robot_datas.end(); it_robot_data++){
		double current_width  = fabsf(center_(0) - it_robot_data->position()(0));
		double current_height = fabsf(center_(1) - it_robot_data->position()(1));
		double current_depth  = fabsf(center_(2) - it_robot_data->position()(2));



		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;
	}


	// Check if the width is the highest value and assign that for the cube dimension

	if(max_width > max_height && max_width > max_depth){

		width_ = max_width;

	// Check if the height is the heighest value and assign that for the cube dimension
	}else if(max_height > max_width && max_height > max_depth) {

		width_ = max_height;

	// Else it must be the depth or it's the same value as some of the other ones
	} else {

		width_ = max_depth;
	}

	// We have to multiply the width by 2 to get real width
	width_ *= 2.0;
}


Vector3d Octree::OctreeNode::new_node_center(Vector3d center, double width, int node_id){


	// Initialize the new node center

	Vector3d node_center;

	// Switch on the ID to see which subdivided node we are finding the center
	switch(node_id) {

		case TOP_LEFT_FRONT:

			// Calculate the center of this new node
			node_center(0) = center(0) - width/4;
			node_center(1) = center(1) + width/4;
			node_center(2) = center(2) + width/4;
			break;



		case TOP_LEFT_BACK:

			// Calculate the center of this new node
			node_center(0) = center(0) - width/4;
			node_center(1) = center(1) + width/4;
			node_center(2) = center(2) - width/4;
			break;


		case TOP_RIGHT_BACK:

			// Calculate the center of this new node
			node_center(0) = center(0) + width/4;
			node_center(1) = center(1) + width/4;
			node_center(2) = center(2) - width/4;
			break;


		case TOP_RIGHT_FRONT:

			// Calculate the center of this new node
			node_center(0) = center(0) + width/4;
			node_center(1) = center(1) + width/4;
			node_center(2) = center(2) + width/4;
			break;



		case BOTTOM_LEFT_FRONT:

			// Calculate the center of this new node
			node_center(0) = center(0) - width/4;
			node_center(1) = center(1) - width/4;
			node_center(2) = center(2) + width/4;
			break;



		case BOTTOM_LEFT_BACK:

			// Calculate the center of this new node
			node_center(0) = center(0) - width/4;
			node_center(1) = center(1) - width/4;
			node_center(2) = center(2) - width/4;
			break;



		case BOTTOM_RIGHT_BACK:

			// Calculate the center of this new node
			node_center(0) = center(0) + width/4;
			node_center(1) = center(1) - width/4;
			node_center(2) = center(2) - width/4;
			break;



		case BOTTOM_RIGHT_FRONT:

			// Calculate the center of this new node
			node_center(0) = center(0) + width/4;
			node_center(1) = center(1) - width/4;
			node_center(2) = center(2) + width/4;
			break;

	}


	return node_center;

}



void Octree::OctreeNode::create_new_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
							 const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
							 const std::vector< boost::shared_ptr<RobotData> > & robot_datas,
							 Vector3d center,
					  	     double width,
					  	     int node_id) {



		octree_nodes_[node_id] = boost::shared_ptr<OctreeNode>( new OctreeNode() );

		Vector3d node_center = new_node_center(center, width, node_id);


		// Increase the current level of subdivision
		octree_nodes_[node_id]->set_level(level_ + 1);

		// Set some tree specific attributes.
		octree_nodes_[node_id]->set_max_levels(max_levels_);
		octree_nodes_[node_id]->set_min_width(min_width_);

		// When there are no objects added to this node, dont't create it
			if(markers.size() + obstacles.size() + robot_datas.size() < 1 ){
				return;
			}

		// Recurse through this node and subdivide it if necessary
		octree_nodes_[node_id]->setup_node(markers, obstacles, robot_datas, node_center, width / 2);

}




void Octree::OctreeNode::setup_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
						  const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
						  const std::vector< boost::shared_ptr<RobotData> > & robot_datas,
						  Vector3d center,
						  double width) {


	center_ = center;

	width_ = width;
	double squared_width = width * width;

	int object_count = markers.size() + obstacles.size() + robot_datas.size();


	// Check if we have any objects in this node and we haven't subdivided
	// above our max depth and the actual width is greater than the minimal width.

	if( (object_count > 0) && (width > min_width_) && (level_ < max_levels_) )
	{

		//Set sub_divided_ to true, so we no there are no robots or markers addedto this node
		// But there might be some obstacles stored in this node.

		sub_divided_ = true;


		// Create a new vector for obstacles, robot_datas and markers for each new possible node
		// Those will be passed to the create_node method.

		// TOP_LEFT_FRONT node list
		std::vector<boost::shared_ptr<Obstacle> > obstaclesA[8];
		std::vector<boost::shared_ptr<RobotData> > robot_dataA[8];
		std::vector<boost::shared_ptr<WorldObject> > markersA[8];



		//Check to which sub node each object belongs and add them to the corresponding vector.
		BOOST_FOREACH( boost::shared_ptr<RobotData> it_robot_data, robot_datas){
		//std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot_data;
		//for(it_robot_data = robot_datas.begin(); it_robot_data != robot_datas.end(); ++it_robot_data){
			Vector3d pos = it_robot_data->position();

			int child_num = this->point_in_node( pos );
			robot_dataA[ child_num ].push_back( it_robot_data );


		}

		// The same for markers
		//std::vector<boost::shared_ptr<WorldObject> >::const_iterator it_markers;
		BOOST_FOREACH(boost::shared_ptr<WorldObject> it_markers, markers ){
		//for(it_markers = markers.begin(); it_markers != markers.end(); ++it_markers){
			Vector3d pos = it_markers->position();

			int child_num = point_in_node( pos );
			markersA[ child_num ].push_back( it_markers );

		}

		// The same for obstacles.


		Vector3d centers[8];
		for(int i = 0; i < 8; ++i ){
			centers[i] = new_node_center(center_,width_, i );
		}

		//std::vector<boost::shared_ptr<Obstacle> >::const_iterator it_obstacles;
		BOOST_FOREACH( boost::shared_ptr<Obstacle> it_obstacles, obstacles ){
		//for(it_obstacles = obstacles.begin(); it_obstacles != obstacles.end(); ++it_obstacles){
			Vector3d pos = it_obstacles->position();

			double max_size = it_obstacles->max_dimension();

			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ){

				double x = centers[0](0) - pos(0);
				double y = centers[0](1) - pos(1);
				double z = centers[0](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[0].push_back( it_obstacles );

					continue;
				}
			}

			// Check if the point lines within the TOP LEFT BACK node
			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {
				double x = centers[1](0) - pos(0);
				double y = centers[1](1) - pos(1);
				double z = centers[1](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[1].push_back( it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the TOP RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {

				double x = centers[2](0) - pos(0);
				double y = centers[2](1) - pos(1);
				double z = centers[2](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[2].push_back( it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the TOP RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ) {

				double x = centers[3](0) - pos(0);
				double y = centers[3](1) - pos(1);
				double z = centers[3](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[3].push_back( it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the BOTTOM LEFT FRONT node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {

				double x = centers[4](0) - pos(0);
				double y = centers[4](1) - pos(1);
				double z = centers[4](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[4].push_back( it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the BOTTOM LEFT BACK node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {

				double x = centers[5](0) - pos(0);
				double y = centers[5](1) - pos(1);
				double z = centers[5](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[5].push_back( it_obstacles);

					continue;
				}
			}


			// Check if the point lines within the BOTTOM RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {

				double x = centers[6](0) - pos(0);
				double y = centers[6](1) - pos(1);
				double z = centers[6](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[6].push_back( it_obstacles);

					continue;
				}
			}


			// Check if the point lines within the BOTTOM RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {

				double x = centers[7](0) - pos(0);
				double y = centers[7](1) - pos(1);
				double z = centers[7](2) - pos(2);
				if( x*x + y*y + z*z + max_size < squared_width / 4 ){
					obstaclesA[7].push_back( it_obstacles);

					continue;
				}
			}

			// The obstacles does not fit to any subnode, so add it to this node.
			object_count_++;
			obstacles_.push_back( it_obstacles);

		}




		// Create the new nodes
		for(int i = 0; i < 8; i++){
			create_new_node(markersA[i], obstaclesA[i], robot_dataA[i], center_, width_, i );
		}




		for(int i = 0; i < 8; i++){
			octree_nodes_[i]->set_parent(this);
		}

	} else {

		//If we get here, there were nod objects or the max level was reached or the width would be to small.
		sub_divided_ = false;
		// assign all objects to this node.
		assign_objects_to_node(markers, obstacles, robot_datas);
		object_count_ = object_count;

	}


}

double Octree::OctreeNode::calculate_dist_to_node(const Vector3d &pos) const {


	double radius = std::sqrt(width_*width_ /2.0);

	double x = pos(0) - center_(0);
	double y = pos(1) - center_(1);
	double z = pos(2) - center_(2);

	return std::sqrt(x*x + y*y + z*z ) - radius;
}



int Octree::OctreeNode::point_in_node(const Vector3d & pos){
	if( (pos(0) <= center_(0)) && (pos(1) >= center_(1)) && (pos(2) >= center_(2)) ){
			return TOP_LEFT_FRONT;
	}

	// Check if the point lines within the TOP LEFT BACK node
	if( (pos(0) <= center_(0)) && (pos(1) >= center_(1)) && (pos(2) <= center_(2)) ) {
		return TOP_LEFT_BACK;
	}

	// Check if the point lines within the TOP RIGHT BACK node

	if( (pos(0) >= center_(0)) && (pos(1) >= center_(1)) && (pos(2) <= center_(2)) ) {
		return TOP_RIGHT_BACK;
	}

	// Check if the point lines within the TOP RIGHT FRONT node

	if( (pos(0) >= center_(0)) && (pos(1) >= center_(1)) && (pos(2) >= center_(2)) ) {
		return TOP_RIGHT_FRONT;
	}

	// Check if the point lines within the BOTTOM LEFT FRONT node

	if( (pos(0) <= center_(0)) && (pos(1) <= center_(1)) && (pos(2) >= center_(2)) ) {
		return BOTTOM_LEFT_FRONT;
	}

	// Check if the point lines within the BOTTOM LEFT BACK node

	if( (pos(0) <= center_(0)) && (pos(1) <= center_(1)) && (pos(2) <= center_(2)) ) {
		return BOTTOM_LEFT_BACK;
	}


	// Check if the point lines within the BOTTOM RIGHT BACK node

	if( (pos(0) >= center_(0)) && (pos(1) <= center_(1)) && (pos(2) <= center_(2)) ) {
		return BOTTOM_RIGHT_BACK;
	}


	// Check if the point lines within the BOTTOM RIGHT FRONT node

	if( (pos(0) >= center_(0)) && (pos(1) <= center_(1)) && (pos(2) >= center_(2)) ) {
		return BOTTOM_RIGHT_FRONT;
	}

   // Never returned
	return -1;
}

void Octree::create_tree(const std::vector< boost::shared_ptr<WorldObject> > & markers,
					 const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
					 const std::vector< boost::shared_ptr<RobotData> > & robot_datas){

	root_ = boost::shared_ptr<OctreeNode>( new OctreeNode() );

	root_->set_max_levels( max_levels_);
	root_->set_min_width( min_width_ );

	root_->scene_dimensions(markers, obstacles, robot_datas);


	root_->setup_node(markers,obstacles, robot_datas, root_->center(), root_->width());

}


void Octree::OctreeNode::assign_objects_to_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
									const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
									const std::vector< boost::shared_ptr<RobotData> > & robot_datas) {

	markers_.insert(markers_.end(),markers.begin(), markers.end());
	obstacles_.insert(obstacles_.end(), obstacles.begin(), obstacles.end());
	robot_datas_.insert(robot_datas_.end() , robot_datas.begin(), robot_datas.end() );
}

void Octree::OctreeNode::add_marker_to_node(boost::shared_ptr<WorldObject> marker) {

	markers_.push_back(marker);

}

void Octree::OctreeNode::add_obstacle_to_node(boost::shared_ptr<Obstacle> obstacle){


	obstacles_.push_back(obstacle);

}

void Octree::OctreeNode::add_robot_data_to_node(boost::shared_ptr<RobotData> robot_data){

	robot_datas_.push_back(robot_data);

}



