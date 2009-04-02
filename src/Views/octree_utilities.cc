/*
 * octree_utilities.cc
 *
 *  Created on: 22.01.2009
 *      Author: kamil
 */

#include <cmath>
#include <vector>
#include <iostream>


#include "../Model/obstacle.h"
#include "../Model/robot.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"
#include "../Model/marker_information.h"
#include "../Model/world_object.h"
#include "../Model/sphere.h"
#include "../Model/box.h"

#include "../Utilities/vector_arithmetics.h"

#include "octree.h"
#include "octree_utilities.h"






bool OctreeUtilities::compare_to_squared_radius(const boost::shared_ptr<const OctreeNode>& octree,const Vector3d& pos, double sq_radius){

	Vector3d center = octree->center();
	double x = center(0) - pos(0);
	double y = center(1) - pos(1);
	double z = center(2) - pos(2);

	return (x*x + y*y + z*z)*(1 - 2*octree->width()) + octree->width() *octree->width()/2.0 <  sq_radius ;

}


double OctreeUtilities::calculate_squared_dist(const boost::shared_ptr<const Octree::OctreeNode>& octree,const Vector3d& pos){
	Vector3d center = octree->center();
		double x = center(0) - pos(0);
		double y = center(1) - pos(1);
		double z = center(2) - pos(2);

		return x*x + y*y + z*z ;
}



std::vector<OctreeUtilities::RobotRef > OctreeUtilities::get_visible_robots_by_radius(const boost::shared_ptr<const Octree> &octree,
																				    const Vector3d & pos,
																				    double view_radius,
																				    const RobotData &robot){

	std::vector<OctreeUtilities::RobotRef > found_robots;


	// do the real work
	get_visible_robots_by_radius_Rec(octree->root(), found_robots, view_radius*view_radius, pos);

	// remove caller from set
	found_robots.erase(std::find(found_robots.begin(), found_robots.end(), boost::static_pointer_cast<RobotIdentifier>(robot.id())));

	return found_robots;
}

 void OctreeUtilities::get_visible_robots_by_radius_Rec(const boost::shared_ptr<const OctreeNode> &octree,
														std::vector<OctreeUtilities::RobotRef > & robots_found,
													    double sq_radius,
													    const Vector3d & pos ) {


	 // check all robots in this node
	 if(octree->robot_datas().size() > 0){

		 // get robot datas and iterator
		 vector<boost::shared_ptr<RobotData> > robots = octree->robot_datas();
		 vector<boost::shared_ptr<RobotData> >::const_iterator it_robots;

		 for(it_robots = robots.begin(); it_robots != robots.end(); it_robots++ ){

			 const Vector3d& rob_pos = (*it_robots)->position();

			 double x = rob_pos(0) - pos(0);
			 double y = rob_pos(1) - pos(1);
			 double z = rob_pos(2) - pos(2);

			 // check whether this robot is near enough
			 if(x*x + y*y + z*z <= sq_radius){

				 robots_found.push_back(boost::static_pointer_cast<RobotIdentifier>((*it_robots)->id()));
			 }
		 }


	 }

	 // is this a leaf?
	 if(octree->sub_divided()){

		 // visit each node which has a distance at most the radius
		 for(int child_num = 0; child_num < 8; child_num++){

			 if(compare_to_squared_radius(octree->child( child_num ),pos,sq_radius)){

				 get_visible_robots_by_radius_Rec(octree->child( child_num ), robots_found, sq_radius, pos);
			 }

		 }


	 }

 }

 std::vector<OctreeUtilities::MarkerRef> OctreeUtilities::get_visible_markers_by_radius(const boost::shared_ptr<const Octree> &octree,
																					 const Vector3d & pos,
																					 double view_radius) {

 	std::vector<OctreeUtilities::MarkerRef > found_markers;

 	get_visible_markers_by_radius_Rec(octree->root(), found_markers, view_radius*view_radius, pos);

 	return found_markers;
 }


  void OctreeUtilities::get_visible_markers_by_radius_Rec(const boost::shared_ptr<const OctreeNode> &octree,
														  std::vector<OctreeUtilities::MarkerRef > & markers_found,
 													      double sq_radius,
 													      const Vector3d & pos ) {


 	 if(octree->marker_information().size() > 0){
 		 vector<boost::shared_ptr<WorldObject> > markers = octree->marker_information();
 		 vector<boost::shared_ptr<WorldObject> >::const_iterator it_markers;
 		 for(it_markers = markers.begin(); it_markers != markers.end(); it_markers++ ){

 			 Vector3d rob_pos = (*it_markers)->position();

 			 double x = rob_pos(0) - pos(0);
 			 double y = rob_pos(1) - pos(1);
 			 double z = rob_pos(2) - pos(2);

 			 if(x*x + y*y + z*z < sq_radius){

 				 markers_found.push_back(boost::static_pointer_cast<MarkerIdentifier>((*it_markers)->id()));
 			 }
 		 }


 	 }

 	 if(octree->sub_divided()){

 		 for(int child_num = 0; child_num < 8; child_num++){

 			 if(compare_to_squared_radius(octree->child( child_num),pos,sq_radius)){

 				 get_visible_markers_by_radius_Rec(octree->child(child_num), markers_found, sq_radius, pos);
 			 }

 		 }


 	 }

  }



  std::vector<OctreeUtilities::ObstacleRef> OctreeUtilities::get_visible_obstacles_by_radius( const boost::shared_ptr<const Octree> &octree,
																						   const Vector3d & pos,
																						   double view_radius) {

  	std::vector<OctreeUtilities::ObstacleRef > found_obstacles;

  	get_visible_obstacles_by_radius_Rec(octree->root(), found_obstacles, view_radius, pos);

  	return found_obstacles;
  }


 void OctreeUtilities::get_visible_obstacles_by_radius_Rec(const boost::shared_ptr<const OctreeNode> &octree,
														   std::vector<OctreeUtilities::ObstacleRef > & obstacles_found,
  													       double radius,
  													       const Vector3d & pos ) {


  	 if(octree->marker_information().size() > 0){
  		 vector<boost::shared_ptr<Obstacle> > obstacles = octree->obstacles() ;
  		 vector<boost::shared_ptr<Obstacle> >::const_iterator it_obstacles;
  		 for(it_obstacles = obstacles.begin(); it_obstacles != obstacles.end(); it_obstacles++ ){

  			 Vector3d rob_pos = (*it_obstacles)->position();

  			 double x = rob_pos(0) - pos(0);
  			 double y = rob_pos(1) - pos(1);
  			 double z = rob_pos(2) - pos(2);

  			 double obstacle_width = (*it_obstacles)->max_dimension();
  			 double r = radius + obstacle_width;
  			 r*=r;

  			 if(x*x + y*y + z*z < r){

  				 obstacles_found.push_back(boost::static_pointer_cast<ObstacleIdentifier>((*it_obstacles)->id()));
  			 }
  		 }


  	 }

  	 if(octree->sub_divided()){

  		 for(int child_num = 0; child_num < 8; child_num++){

  			 if(compare_to_squared_radius(octree->child( child_num ) , pos, radius)){

  				 get_visible_obstacles_by_radius_Rec(octree->child(child_num), obstacles_found, radius, pos);
  			 }

  		 }


  	 }

   }


 std::vector<OctreeUtilities::RobotRef> OctreeUtilities::get_nearest_robots(const boost::shared_ptr<const Octree> &octree,
																		 const Vector3d &pos,
																		 const RobotRef & id,
																		 std::size_t num_nearest) {

	 // create an empty queue
	 PriorityQueue<RobotRef>::Type queue;

	 // find nearest robots
	 get_nearest_robots_Rec(octree->root(), pos,num_nearest, queue, id);

	 // calculate the set
	 std::vector<RobotRef> found_robots;

	 while(!queue.empty() ){
		 found_robots.push_back( RobotRef(queue.top()->id() ) );
		 queue.pop();

	 }

	 return found_robots;


 }

void OctreeUtilities::get_nearest_robots_Rec(const boost::shared_ptr<const OctreeNode>& octree,
											 const Vector3d &pos,
											 std::size_t num_nearest,
											 PriorityQueue<RobotRef>::Type & queue,
											 const RobotRef & id){

	// leaf
	 if( octree->sub_divided() == false ){

		 // get the robot datas stored in this node and an iterator for them
		 std::vector<boost::shared_ptr<RobotData> > robots = octree->robot_datas();

		 std::vector<boost::shared_ptr<RobotData> >::iterator it_robots;

		 std::size_t num_robots_found = queue.size();

		 double farest_dist = std::numeric_limits<double>::max();

		 if(num_robots_found > 0){
			 farest_dist  = queue.top()->dist();
		 }

		 // check all robots in this node
		 for(it_robots = robots.begin(); it_robots != robots.end(); ++it_robots){

			 // don't add the id of the robot we are doing the search for
			 if((*it_robots)->id() == id) {
				 continue;
			 }

			 Vector3d robot_pos = (*it_robots)->position();
			 double x = pos(0) - robot_pos(0);
			 double y = pos(1) - robot_pos(1);
			 double z = pos(2) - robot_pos(2);

			 double dist = std::sqrt(x*x + y*y + z*z );

			 if(num_robots_found < num_nearest){

				 boost::shared_ptr<QueueEntry<RobotRef> > new_entry =
										 boost::shared_ptr<QueueEntry<RobotRef> >
										 (new QueueEntry<RobotRef>(boost::static_pointer_cast<RobotIdentifier>((*it_robots)->id()), dist) );

				 queue.push( new_entry );
				 farest_dist = queue.top()->dist();
				 num_robots_found++;

			 }else if(dist < farest_dist){

				 boost::shared_ptr<QueueEntry<RobotRef> > new_entry =
										 boost::shared_ptr<QueueEntry<RobotRef> >
												 (new QueueEntry<RobotRef>(boost::static_pointer_cast<RobotIdentifier>((*it_robots)->id()), dist) );

				 queue.pop();
				 queue.push( new_entry );

				 farest_dist  = queue.top()->dist();
			 }



		 }


	 } else { // not a leaf

		 // get the number f the child we are in,
		 // so we visit the nearest node first
		 int child_iam_in = octree->point_in_node(pos);

		 get_nearest_robots_Rec(octree->child( child_iam_in ), pos, num_nearest, queue,id );

		 // we might have to check other nodes too
		 for( int num_child = 0; num_child < 8; ++num_child ){

			 // check whether we have visited this node already
			 if(num_child == child_iam_in)
				 continue;

			 // until now we have not found enough robots
			 // so we have to visit the cild anyway
			 if(queue.size() <  num_nearest ){

				 get_nearest_robots_Rec(octree->child( num_child ), pos, num_nearest,queue, id );

			 } else {
				 // get the distance to the robot which is farest away
				 double farest_dist = queue.top()->dist();

				 // only visit the child if there might be a nearer robot than thos found so far
				 if(octree->child( num_child )->calculate_dist_to_node( pos ) < farest_dist ){
					 get_nearest_robots_Rec( octree->child( num_child ) , pos, num_nearest, queue, id );
				 }

			 }
		 }



	 }

 }

std::vector<OctreeUtilities::MarkerRef> OctreeUtilities::get_nearest_markers(const boost::shared_ptr<const Octree> &octree,
																		 const Vector3d &pos,
																		 std::size_t num_nearest) {

	 // create an empty queue
	 PriorityQueue<MarkerRef>::Type queue;

	 // find nearest robots
	 get_nearest_markers_Rec(octree->root(), pos,num_nearest, queue);

	 // calculate the set
	 std::vector<MarkerRef> found_markers;

	 while(!queue.empty() ){
		 found_markers.push_back( MarkerRef(queue.top()->id() ) );
		 queue.pop();

	 }


	 return found_markers;


 }

void OctreeUtilities::get_nearest_markers_Rec(const boost::shared_ptr<const OctreeNode>& octree,
											 const Vector3d &pos,
											 std::size_t num_nearest,
											 PriorityQueue<MarkerRef>::Type & queue){

	// leaf
	 if( octree->sub_divided() == false ){

		 // get the robot datas stored in this node and an iterator for them
		 std::vector<boost::shared_ptr<WorldObject> > markers = octree->marker_information();

		 std::vector<boost::shared_ptr<WorldObject> >::iterator it_markers;

		 std::size_t num_markers_found = queue.size();

		 double farest_dist = std::numeric_limits<double>::max();

		 if(num_markers_found > 0){
			 farest_dist  = queue.top()->dist();
		 }

		 // check all robots in this node
		 for(it_markers = markers.begin(); it_markers != markers.end(); ++it_markers){

			 Vector3d robot_pos = (*it_markers)->position();
			 double x = pos(0) - robot_pos(0);
			 double y = pos(1) - robot_pos(1);
			 double z = pos(2) - robot_pos(2);

			 double dist = std::sqrt(x*x + y*y + z*z );

			 if(num_markers_found < num_nearest){

				 boost::shared_ptr<QueueEntry<MarkerRef> > new_entry =
									boost::shared_ptr<QueueEntry<MarkerRef> >
											(new QueueEntry<MarkerRef>(
													boost::static_pointer_cast<MarkerIdentifier>((*it_markers)->id())
													, dist) );

				 queue.push(new_entry );
				 farest_dist = queue.top()->dist();
				 num_markers_found++;

			 }else if(dist < farest_dist){

				 boost::shared_ptr<QueueEntry<MarkerRef> > new_entry =
									boost::shared_ptr<QueueEntry<MarkerRef> >
													(new QueueEntry<MarkerRef>(
														 boost::static_pointer_cast<MarkerIdentifier>((*it_markers)->id())
														, dist) );
				 queue.pop();
				 queue.push( new_entry );

				 farest_dist  = queue.top()->dist();
			 }



		 }


	 } else { // not a leaf

		 // get the number f the child we are in,
		 // so we visit the nearest node first
		 int child_iam_in = octree->point_in_node(pos);

		 get_nearest_markers_Rec(octree->child( child_iam_in ), pos, num_nearest, queue );

		 // we might have to check other nodes too
		 for( int num_child = 0; num_child < 8; ++num_child ){

			 // check whether we have visited this node already
			 if(num_child == child_iam_in)
				 continue;

			 // until now we have not found enough robots
			 // so we have to visit the cild anyway
			 if(queue.size() <  num_nearest ){

				 get_nearest_markers_Rec(octree->child( num_child ), pos, num_nearest,queue );

			 } else {
				 // get the distance to the robot which is farest away
				 double farest_dist = queue.top()->dist();

				 // only visit the child if there might be a nearer robot than thos found so far
				 if(octree->child( num_child )->calculate_dist_to_node( pos ) < farest_dist ){
					 get_nearest_markers_Rec( octree->child( num_child ) , pos, num_nearest, queue );
				 }

			 }
		 }



	 }

 }



std::vector<OctreeUtilities::ObstacleRef> OctreeUtilities::get_nearest_obstacles(const boost::shared_ptr<const Octree> &octree,
																		 const Vector3d &pos,
																		 std::size_t num_nearest) {

	 // create an empty queue
	 PriorityQueue<ObstacleRef>::Type queue;

	 // find nearest robots
	 get_nearest_obstacles_Rec(octree->root(), pos,num_nearest, queue);

	 // calculate the set
	 std::vector<ObstacleRef> found_obstacles;

	 while(!queue.empty() ){
		 found_obstacles.push_back( ObstacleRef(queue.top()->id() ) );
		 queue.pop();

	 }


	 return found_obstacles;


 }


void OctreeUtilities::get_nearest_obstacles_Rec(const boost::shared_ptr<const OctreeNode>& octree,
											 const Vector3d &pos,
											 std::size_t num_nearest,
											 PriorityQueue<ObstacleRef>::Type & queue){

	// leaf
	 if( octree->sub_divided() == false ){

		 // get the robot datas stored in this node and an iterator for them
		 std::vector<boost::shared_ptr<Obstacle> > obstacles = octree->obstacles();

		 std::vector<boost::shared_ptr<Obstacle> >::iterator it_obstacles;

		 std::size_t num_obstacles_found = queue.size();

		 double farest_dist = std::numeric_limits<double>::max();

		 if(num_obstacles_found > 0){
			 farest_dist  = queue.top()->dist();
		 }

		 // check all robots in this node
		 for(it_obstacles = obstacles.begin(); it_obstacles != obstacles.end(); ++it_obstacles){

			 Vector3d robot_pos = (*it_obstacles)->position();
			 double x = pos(0) - robot_pos(0);
			 double y = pos(1) - robot_pos(1);
			 double z = pos(2) - robot_pos(2);

			 double width =  (*it_obstacles)->max_dimension();
			 double dist = std::sqrt(x*x + y*y + z*z ) - width;

			 // not enough found, so we have to add it anyway
			 if(num_obstacles_found < num_nearest){

				 boost::shared_ptr<QueueEntry<ObstacleRef> > new_entry =
									boost::shared_ptr<QueueEntry<ObstacleRef> >
											(new QueueEntry<ObstacleRef>(
													boost::static_pointer_cast<ObstacleIdentifier>((*it_obstacles)->id())
													, dist) );

				 queue.push(new_entry );
				 farest_dist = queue.top()->dist();
				 num_obstacles_found++;
				 //nearer than already found ?
			 }else if(dist < farest_dist){

				 boost::shared_ptr<QueueEntry<ObstacleRef> > new_entry =
									boost::shared_ptr<QueueEntry<ObstacleRef> >
													(new QueueEntry<ObstacleRef>(
														 boost::static_pointer_cast<ObstacleIdentifier>((*it_obstacles)->id())
														, dist) );
				 queue.pop();
				 queue.push( new_entry );

				 farest_dist  = queue.top()->dist();
			 }



		 }


	 } else { // not a leaf

		 // get the number f the child we are in,
		 // so we visit the nearest node first
		 int child_iam_in = octree->point_in_node(pos);

		 get_nearest_obstacles_Rec(octree->child( child_iam_in ), pos, num_nearest, queue );

		 // we might have to check other nodes too
		 for( int num_child = 0; num_child < 8; ++num_child ){

			 // check whether we have visited this node already
			 if(num_child == child_iam_in)
				 continue;

			 // until now we have not found enough robots
			 // so we have to visit the cild anyway
			 if(queue.size() <  num_nearest ){

				 get_nearest_obstacles_Rec(octree->child( num_child ), pos, num_nearest,queue );

			 } else {
				 // get the distance to the robot which is farest away
				 double farest_dist = queue.top()->dist();

				 // only visit the child if there might be a nearer robot than thos found so far
				 if(octree->child( num_child )->calculate_dist_to_node( pos ) < farest_dist ){
					 get_nearest_obstacles_Rec( octree->child( num_child ) , pos, num_nearest, queue );
				 }

			 }
		 }



	 }

 }







