/*
 * octree.cc
 *
 *  Created on: 08.01.2009
 *      Author: kamil
 */

#include <cmath>


#include "../Model/obstacle.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/marker_information.h"
#include "../Model/sphere.h"
#include "../Model/box.h"

#include "octree.h"




Octree::Octree() {

	// Set some dummy values
	max_levels_ = 20;
	min_width_ = 1.0;


    // init octree
	init_octree();

}

Octree::Octree(int max_levels): max_levels_(max_levels), min_width_(1.0) {

	init_octree();
}

Octree::Octree(float min_width): max_levels_(2000), min_width_(min_width) {

	init_octree();
}

Octree::Octree(int max_levels, float min_width): max_levels_(max_levels), min_width_(min_width) {


	//init octree
	init_octree();

}



Octree::~Octree() {

	// Call our destroy function

	destroy_octree();

}





void Octree::init_octree() {

	// Set the subdivided flag to false

	sub_divided_ = false;

	// Set the dimensions of the box to false
	width_ = 0.0;

	// Initialize the triangle count

	object_count_ = 0;



	// Initialize the center of the box to the 0

	center_ = Vector3d();

	// Set the sub nodes to NULL



}






void Octree::scene_dimensions(const std::vector< boost::shared_ptr<WorldObject> > & markers,
							  const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
							  const std::vector< boost::shared_ptr<RobotData> > & robot_datas) {


	float max_width = 0;
	float max_height = 0;
	float max_depth = 0;

	// Return from this function if we passed in bad data

	if(markers.size() + obstacles.size() + robot_datas.size() == 0){
		width_ = 0.0;
	}



	// Sum up all positions to get the center point of all objects

	center_ = Vector3d();
	int num_objects = 0;

	std::vector<boost::shared_ptr<WorldObject> >::const_iterator it_markers;
	for(it_markers = markers.begin(); it_markers < markers.end(); it_markers++){
		center_+= (*it_markers)->position();
		num_objects++;

	}

	std::vector< boost::shared_ptr<Obstacle> >::const_iterator it_obstacles;
	for(it_obstacles = obstacles.begin(); it_obstacles < obstacles.end(); it_obstacles++){
		center_ +=  (*it_obstacles)->position();
		num_objects++;
	}

	std::vector< boost::shared_ptr<RobotData> >::const_iterator it_robot_data;

	for(it_robot_data = robot_datas.begin(); it_robot_data < robot_datas.end(); it_robot_data++){
		center_ += (*it_robot_data)->position();
		num_objects++;
	}





	// Divide the total by the number of objects to get the center point.
	// But check first if there were objects at all.
	if(num_objects > 0){
		center_ /= num_objects;
	}




	// Calculate the fartest distance to the center point.
	// Calculate the distance to the center point for all objects
	for(it_markers = markers.begin(); it_markers < markers.end(); it_markers++){
		float current_width  = fabsf(center_(0) - (*it_markers)->position()(0));
		float current_height = fabsf(center_(1) - (*it_markers)->position()(1));
		float current_depth  = fabsf(center_(2) - (*it_markers)->position()(2));



		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;


	}

	for(it_obstacles = obstacles.begin(); it_obstacles < obstacles.end(); it_obstacles++){
		// Add the max size of he obstacle to width, height and depth.
		float obstacle_max_size = determine_obstacle_max_size( *it_obstacles );

		float current_width  = fabsf(center_(0) - (*it_obstacles)->position()(0)) + obstacle_max_size;
		float current_height = fabsf(center_(1) - (*it_obstacles)->position()(1)) + obstacle_max_size;
		float current_depth  = fabsf(center_(2) - (*it_obstacles)->position()(2)) + obstacle_max_size;



		// Check if the current width value is greater than the max width stored.
		if(current_width  > max_width)	max_width  = current_width;
		// Check if the current height value is greater than the max height stored.
		if(current_height > max_height)	max_height = current_height;
		// Check if the current depth value is greater than the max depth stored.
		if(current_depth > max_depth)	max_depth  = current_depth;


	}

	// Don't forget the robots
	for(it_robot_data = robot_datas.begin(); it_robot_data < robot_datas.end(); it_robot_data++){
		float current_width  = fabsf(center_(0) - (*it_robot_data)->position()(0));
		float current_height = fabsf(center_(1) - (*it_robot_data)->position()(1));
		float current_depth  = fabsf(center_(2) - (*it_robot_data)->position()(2));



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





Vector3d Octree::new_node_center(Vector3d center, float width, int node_id)

{


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


	// Return the new node center
	return node_center;

}



void Octree::create_new_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
							 const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
							 const std::vector< boost::shared_ptr<RobotData> > & robot_datas,
							 Vector3d center,
					  	     float width,
					  	     int node_id) {




		// Create new node
		octree_nodes_[node_id] = boost::shared_ptr<Octree>( new Octree() );



		// Get the new node's center point depending on the nodex_id (which of the 8 subdivided cubes).
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
		octree_nodes_[node_id]->create_node(markers, obstacles, robot_datas, node_center, width / 2);




}





void Octree::create_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
						  const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
						  const std::vector< boost::shared_ptr<RobotData> > & robot_datas,
						  Vector3d center,
						  float width) {




	// Initialize this node's center point.  Now we know the center of this node.
	center_ = center;
	// Initialize this nodes cube width.  Now we know the width of this current node.

	width_ = width;

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
		std::vector<boost::shared_ptr<Obstacle> > obstacles_1;
		std::vector<boost::shared_ptr<RobotData> > robot_data_1;
		std::vector<boost::shared_ptr<WorldObject> > markers_1;

		// TOP_LEFT_BACK node list
		std::vector<boost::shared_ptr<Obstacle> > obstacles_2;
		std::vector<boost::shared_ptr<RobotData> > robot_data_2;
		std::vector<boost::shared_ptr<WorldObject> > markers_2;

		// TOP_RIGHT_BACK node list
		std::vector<boost::shared_ptr<Obstacle> > obstacles_3;
		std::vector<boost::shared_ptr<RobotData> > robot_data_3;
		std::vector<boost::shared_ptr<WorldObject> > markers_3;

		// TOP_RIGHT_FRONT node list
		std::vector<boost::shared_ptr<Obstacle> > obstacles_4;
		std::vector<boost::shared_ptr<RobotData> > robot_data_4;
		std::vector<boost::shared_ptr<WorldObject> > markers_4;

		// BOTTOM_LEFT_FRONT node list
		std::vector<boost::shared_ptr<Obstacle> > obstacles_5;
		std::vector<boost::shared_ptr<RobotData> > robot_data_5;
		std::vector<boost::shared_ptr<WorldObject> > markers_5;

		// BOTTOM_LEFT_BACK node list
		std::vector<boost::shared_ptr<Obstacle> > obstacles_6;
		std::vector<boost::shared_ptr<RobotData> > robot_data_6;
		std::vector<boost::shared_ptr<WorldObject> > markers_6;

		// BOTTOM_RIGHT_BACK node list
		std::vector<boost::shared_ptr<Obstacle> > obstacles_7;
		std::vector<boost::shared_ptr<RobotData> > robot_data_7;
		std::vector<boost::shared_ptr<WorldObject> > markers_7;

		// BOTTOM_RIGHT_FRONT node list
		std::vector<boost::shared_ptr<Obstacle> > obstacles_8;
		std::vector<boost::shared_ptr<RobotData> > robot_data_8;
		std::vector<boost::shared_ptr<WorldObject> > markers_8;


		//Check to which sub node each object belongs and add them to the corresponding vector.
		std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot_data;
		for(it_robot_data = robot_datas.begin(); it_robot_data != robot_datas.end(); ++it_robot_data){
			Vector3d pos = (*it_robot_data)->position();

			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ){
				robot_data_1.push_back(*it_robot_data);
				continue;
			}

			// Check if the point lines within the TOP LEFT BACK node
			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {
				robot_data_2.push_back(*it_robot_data);
				continue;
			}

			// Check if the point lines within the TOP RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {
				robot_data_3.push_back(*it_robot_data);
				continue;
			}

			// Check if the point lines within the TOP RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ) {
				robot_data_4.push_back(*it_robot_data);
				continue;
			}

			// Check if the point lines within the BOTTOM LEFT FRONT node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {
				robot_data_5.push_back(*it_robot_data);
				continue;
			}

			// Check if the point lines within the BOTTOM LEFT BACK node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {
				robot_data_6.push_back(*it_robot_data);
				continue;
			}


			// Check if the point lines within the BOTTOM RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {
				robot_data_7.push_back(*it_robot_data);
				continue;
			}


			// Check if the point lines within the BOTTOM RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {
				robot_data_8.push_back(*it_robot_data);
				continue;
			}


		}

		// The same for markers
		std::vector<boost::shared_ptr<WorldObject> >::const_iterator it_markers;
		for(it_markers = markers.begin(); it_markers != markers.end(); ++it_markers){
			Vector3d pos = (*it_markers)->position();

			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ){
				markers_1.push_back(*it_markers);
				continue;
			}

			// Check if the point lines within the TOP LEFT BACK node
			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {
				markers_2.push_back(*it_markers);
				continue;
			}

			// Check if the point lines within the TOP RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {
				markers_3.push_back(*it_markers);
				continue;
			}

			// Check if the point lines within the TOP RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ) {
				markers_4.push_back(*it_markers);
				continue;
			}

			// Check if the point lines within the BOTTOM LEFT FRONT node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {
				markers_5.push_back(*it_markers);
				continue;
			}

			// Check if the point lines within the BOTTOM LEFT BACK node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {
				markers_6.push_back(*it_markers);
				continue;
			}


			// Check if the point lines within the BOTTOM RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {
				markers_7.push_back(*it_markers);
				continue;
			}


			// Check if the point lines within the BOTTOM RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {
				markers_8.push_back(*it_markers);
				continue;
			}


		}

		// The same for obstacles.

		//TODO(kamil): The code below is tempory, this should be replaced in the near future
		// The test for fitting is not accurate; it is a pessimistic test atm.

		Vector3d centers[8];
		centers[0] = new_node_center(center_, width_, Octree::TOP_LEFT_FRONT);
		centers[1] = new_node_center(center_, width_, Octree::TOP_LEFT_BACK);
		centers[2] = new_node_center(center_, width_, Octree::TOP_RIGHT_BACK);
		centers[3] = new_node_center(center_, width_, Octree::TOP_RIGHT_FRONT);
		centers[4] = new_node_center(center_, width_, Octree::BOTTOM_LEFT_FRONT);
		centers[5] = new_node_center(center_, width_, Octree::BOTTOM_LEFT_BACK);
		centers[6] = new_node_center(center_, width_, Octree::BOTTOM_RIGHT_BACK);
		centers[7] = new_node_center(center_, width_, Octree::BOTTOM_RIGHT_FRONT);


		std::vector<boost::shared_ptr<Obstacle> >::const_iterator it_obstacles;
		for(it_obstacles = obstacles.begin(); it_obstacles != obstacles.end(); ++it_obstacles){
			Vector3d pos = (*it_obstacles)->position();

			float max_size = determine_obstacle_max_size(*it_obstacles);

			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ){

				float x = centers[0](0) - pos(0);
				float y = centers[0](1) - pos(1);
				float z = centers[0](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_1.push_back(*it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the TOP LEFT BACK node
			if( (pos(0) <= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {
				float x = centers[1](0) - pos(0);
				float y = centers[1](1) - pos(1);
				float z = centers[1](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_2.push_back(*it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the TOP RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) <= center(2)) ) {

				float x = centers[2](0) - pos(0);
				float y = centers[2](1) - pos(1);
				float z = centers[2](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_3.push_back(*it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the TOP RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) >= center(1)) && (pos(2) >= center(2)) ) {

				float x = centers[3](0) - pos(0);
				float y = centers[3](1) - pos(1);
				float z = centers[3](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_4.push_back(*it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the BOTTOM LEFT FRONT node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {

				float x = centers[4](0) - pos(0);
				float y = centers[4](1) - pos(1);
				float z = centers[4](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_5.push_back(*it_obstacles);

					continue;
				}
			}

			// Check if the point lines within the BOTTOM LEFT BACK node

			if( (pos(0) <= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {

				float x = centers[5](0) - pos(0);
				float y = centers[5](1) - pos(1);
				float z = centers[5](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_6.push_back(*it_obstacles);

					continue;
				}
			}


			// Check if the point lines within the BOTTOM RIGHT BACK node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) <= center(2)) ) {

				float x = centers[6](0) - pos(0);
				float y = centers[6](1) - pos(1);
				float z = centers[6](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_7.push_back(*it_obstacles);

					continue;
				}
			}


			// Check if the point lines within the BOTTOM RIGHT FRONT node

			if( (pos(0) >= center(0)) && (pos(1) <= center(1)) && (pos(2) >= center(2)) ) {

				float x = centers[7](0) - pos(0);
				float y = centers[7](1) - pos(1);
				float z = centers[7](2) - pos(2);
				if(std::sqrt( x*x + y*y + z*z )+ max_size < width_ / 2 ){
					obstacles_8.push_back(*it_obstacles);

					continue;
				}
			}

			// The obstacles does not fit to any subnode, so add it to this node.
			object_count_++;
			obstacles_.push_back(*it_obstacles);

		}




		// Create the new nodes

		create_new_node(markers_1, obstacles_1, robot_data_1, center_, width_, TOP_LEFT_FRONT);

		create_new_node(markers_2, obstacles_2, robot_data_2, center_, width_, TOP_LEFT_BACK);

		create_new_node(markers_3, obstacles_3, robot_data_3, center_, width_, TOP_RIGHT_BACK);

		create_new_node(markers_4, obstacles_4, robot_data_4, center_, width_, TOP_RIGHT_FRONT);

		create_new_node(markers_5, obstacles_5, robot_data_5, center_, width_, BOTTOM_LEFT_FRONT);

		create_new_node(markers_6, obstacles_6, robot_data_6, center_, width_, BOTTOM_LEFT_BACK);

		create_new_node(markers_7, obstacles_7, robot_data_7, center_, width_, BOTTOM_RIGHT_BACK);

		create_new_node(markers_8, obstacles_8, robot_data_8, center_, width_, BOTTOM_RIGHT_FRONT);



		for(int i = 0; i < 8; i++){
			octree_nodes_[i]->set_parent(this);
		}

	} else {

		//If we get here, there were nod objects or the max level was reached or the width would be to small.
		sub_divided_ = false;
		// assign all objects to this node.
		assign_objects_to_node(markers, obstacles, robot_datas);

	}
	// set the object count
	object_count_ = object_count;
}

float Octree::calculate_dist_to_node(const Vector3d &pos) const {

	//TODO: This does not calculate the correct distance to this node, it is an lower bound for it.
	float radius = std::sqrt(width_*width_ /2.0);

	float x = pos(0) - center_(0);
	float y = pos(1) - center_(1);
	float z = pos(2) - center_(2);

	return std::sqrt(x*x + y*y + z*z ) - radius;
}


bool Octree::does_obstacle_fit(boost::shared_ptr<Obstacle> & obstacle) const {

	float size = determine_obstacle_max_size(obstacle);


	//TODO:

	return false;

}

int Octree::determine_obstacle_type(const boost::shared_ptr<Obstacle> &obstacle) const{

	if( dynamic_cast<Box*>(obstacle.get() ) ){
		return 1;

	} else if(dynamic_cast<Sphere*>(obstacle.get() )){
		return 2;
	}

	return -1;
}

float Octree::determine_obstacle_max_size( const boost::shared_ptr<Obstacle> & obstacle) const{
	int obstacle_type = determine_obstacle_type(obstacle);

	float size = 0.0;

	switch(obstacle_type){
		case 1: {
				Box * box = dynamic_cast<Box*>( obstacle.get() );
				size = box->width();

				if(size < box->depth() ){
					size = box->depth();
				}

				if(size < box->height()){
					size = box->height();
				}

				break;
		}
		case 2: {
				Sphere * sphere = dynamic_cast<Sphere*>( obstacle.get() );
				size = sphere->radius();
				break;
		}
		default:
			break;

	}

	return size;
}

void Octree::create_tree(const std::vector< boost::shared_ptr<WorldObject> > & markers,
					 const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
					 const std::vector< boost::shared_ptr<RobotData> > & robot_datas){

	scene_dimensions(markers, obstacles, robot_datas);

	create_node(markers,obstacles, robot_datas, center_, width_);

}


void Octree::assign_objects_to_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
									const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
									const std::vector< boost::shared_ptr<RobotData> > & robot_datas) {

	markers_.insert(markers_.end(),markers.begin(), markers.end());
	obstacles_.insert(obstacles_.end(), obstacles.begin(), obstacles.end());
	robot_datas_.insert(robot_datas_.end() , robot_datas.begin(), robot_datas.end() );
}

void Octree::add_marker_to_node(boost::shared_ptr<WorldObject> marker) {

	markers_.push_back(marker);

}

void Octree::add_obstacle_to_node(boost::shared_ptr<Obstacle> obstacle){


	obstacles_.push_back(obstacle);

}
void Octree::add_robot_data_to_node(boost::shared_ptr<RobotData> robot_data){

	robot_datas_.push_back(robot_data);

}



void Octree::destroy_octree() {

	// Free the triangle data if it's not NULL



//TODO

	// Initialize the octree data members

	init_octree();

}

