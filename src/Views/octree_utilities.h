/*
 * octree_utilities.h
 *
 *  Created on: 19.01.2009
 *      Author: kamil
 */

#ifndef OCTREE_UTILITIES_H_
#define OCTREE_UTILITIES_H_

#include <set>
#include <queue>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "../Model/robot_identifier.h"
#include "../Model/obstacle_identifier.h"
#include "../Model/marker_identifier.h"

#include "../Utilities/vector_arithmetics.h"

#include "octree.h"
#include "view.h"




/**
 * \class OctreeUtilities
 *
 * This Class adds some Methods for searching in the octree:
 *      * by maximal radius
 *      * k Nearest
 *
 */
class OctreeUtilities{

protected:
	// Some typedefs to save work
	typedef boost::shared_ptr<RobotIdentifier> RobotRef ;
	typedef boost::shared_ptr<ObstacleIdentifier> ObstacleRef;
	typedef boost::shared_ptr<MarkerIdentifier> MarkerRef;

public:

	/**
	 * Returns the num_nearest robots relative to a point. To speed up the computation
	 * this works on the octree used by some View Objects.
	 *
	 * \param octree The Octree in which all robots are saved.
	 * \param pos The position for which to calculate the nearest robots
	 * \param num_nearest The number of nearest robots to find
	 *
	 * \return A set of Roboter Identifiers
	 */
	static std::vector<RobotRef> get_nearest_robots( const boost::shared_ptr<const Octree> &octree,
												  const Vector3d &pos,
												  const RobotRef & id,
												  std::size_t num_nearest);

	/**
	 * Returns the num_nearest robots relative to a point. To speed up the computation
	 * this works on the octree used by some View Objects.
	 *
	 * \param octree The Octree in which all robots are saved.
	 * \param pos The position for which to calculate the nearest markers
	 * \param num_nearest The number of nearest markers to find
	 *
	 * \return A set of Roboter Identifiers
	 */
	static std::vector<MarkerRef> get_nearest_markers( const boost::shared_ptr<const Octree> &octree,
													const Vector3d &pos,
													std::size_t num_nearest );


	/**
	 * Returns the num_nearest obstacles relative to a point. To speed up the computation
	 * this works on the octree used by some View Objects.
	 *
	 * \param octree The Octree in which all robots are saved.
	 * \param pos The position for which to calculate the nearest obstacles
	 * \param num_nearest The number of nearest obstacles to find
	 *
	 * \return A set of Obstacle Identifiers
	 */
	static std::vector<ObstacleRef> get_nearest_obstacles( const boost::shared_ptr<const Octree> &octree,
														const Vector3d &pos,
														std::size_t num_nearest );




	/**
	 * Returns the robots which have a distance to the point 'pos' at most view_radius. To speed up the computation this works on
	 * an octree used by some View Objects.
	 *
	 * \param octree The octree to work on
	 * \param pos The position to calculate the distance from
	 * \param view_radius The maximal distance from the point pos
	 *
	 * \return A set of Robot Identifiers.
	 *
	 */
	static std::vector<RobotRef> get_visible_robots_by_radius( const boost::shared_ptr<const Octree> &octree,
															const Vector3d & pos,
															double view_radius,
															const RobotData & robot);

	/**
	 * Returns the markers which have a distance to the point 'pos' at most view_radius. To speed up the computation this works on
	 * an octree used by some View Objects.
	 *
	 * \param octree The octree to work on
	 * \param pos The position to calculate the distance from
	 * \param view_radius The maximal distance from the point pos
	 *
	 * \return A set of Marker Identifiers.
	 *
	 */
	static std::vector<MarkerRef> get_visible_markers_by_radius(   const boost::shared_ptr<const Octree> &octree,
																const Vector3d & pos,
																double view_radius);

	/**
	 * Returns the obstacles which have a distance to the point 'pos' at most view_radius. To speed up the computation this works on
	 * an octree used by some View Objects.
	 *
	 * \param octree The octree to work on
	 * \param pos The position to calculate the distance from
	 * \param view_radius The maximal distance from the point pos
	 *
	 * \return A set of Obstacle Identifiers.
	 *
	 */
	static std::vector<ObstacleRef> get_visible_obstacles_by_radius(const boost::shared_ptr<const Octree> &octree, const Vector3d &pos, double view_radius);

private:

	/**
	 * Do not allow any Objects of this type. So the constructor is private.
	 */
	OctreeUtilities(){};

	/**
	 * Helper Class for the Priority Queue. This represents an entry in
	 * the Priority Queue. It consits of an robot and its distance to the
	 * point.
	 *
	 */
	template<class T>
	class QueueEntry{
	  public:

		/**
		 * This initializes the Entry
		 * \param robot A Robot Identifier
		 * \param dist The distance between the robot and the point.
		 */
		QueueEntry(const T & id, double dist): id_(id), dist_(dist){};

		/**
		 * Returns the robot Identifier
		 */
		const T & id(){return id_; }

		/**
		 * Returns its distance to the point
		 */
		double dist(){return dist_; }

		/**
		* Comparator class so the priority queue is a max heap.
		*/

		class Less{

		 public:
			/**
			 * Overloaded Operator for comparesion of two QueueEntries.
			 * The decision is made up by the distance to the search point
			 */
			bool operator() (const boost::shared_ptr<QueueEntry<T> > & entry_one, const boost::shared_ptr<QueueEntry<T> > & entry_two){

				return entry_one->dist() < entry_two->dist();
			}

		};


	  private:
		/**
		 * The Robot Identifier
		 */
		T id_;

		/**
		 * The distance to the search point
		 */
		double dist_;

	};



	// A small typedef to save work
	template<class T>
	struct PriorityQueue {
		typedef typename QueueEntry<T>::Less Less;
		typedef std::priority_queue<boost::shared_ptr<QueueEntry<T> > , std::vector<boost::shared_ptr<QueueEntry<T> > >, Less > Type;
	};

	/**
	 * Does the real work for computing the nearest robots.
	 *
	 * \param octree The octree to work on
	 * \param pos The position to calculate the nearest robots for.
	 * \param num_nearest The number of robots to find.
	 * \param queue A Max Heap with the already found robots. New Robots will be added to this Heap.
	 * \param id the id of the Robot, so we can distinguish the robot from the other
	 */
	static void get_nearest_robots_Rec( const boost::shared_ptr<const Octree::OctreeNode> &octree,
										const Vector3d &pos,
									    std::size_t num_nearest,
									    PriorityQueue<RobotRef>::Type & queue,
									    const RobotRef & id);

	/**
	 * Does the real work for computing the nearest markers.
	 *
	 * \param octree The octree to work on
	 * \param pos The position to calculate the markers robots for.
	 * \param num_nearest The number of markers to find.
	 * \param queue A Max Heap with the already found markers. New markers will be added to this Heap.
	 */
	static void get_nearest_markers_Rec(const boost::shared_ptr<const OctreeNode> &octree,
										const Vector3d &pos,
									    std::size_t num_nearest,
									    PriorityQueue<MarkerRef>::Type & queue);


	/**
	 * Does the real work for computing the nearest obstacles.
	 *
	 * \param octree The octree to work on
	 * \param pos The position to calculate the nearest obstacles for.
	 * \param num_nearest The number of obstacles to find.
	 * \param queue A Max Heap with the already found obstacles. New obstacles will be added to this Heap.
	 */
	static void get_nearest_obstacles_Rec(  const boost::shared_ptr<const OctreeNode> &octree,
											const Vector3d &pos,
											std::size_t num_nearest,
											PriorityQueue<ObstacleRef>::Type & queue);


	/**
	 * Does the real Work on finding robots by radius.
	 *
	 * \param octree The octree to work on
	 * \param robots_found The set of already found robots. New found robots will be added to
	 * 		 this set.
	 * \param  sq_radius The square of the view radius
	 * \param pos The position of the robot
	 *
	 *
	 */
	static void get_visible_robots_by_radius_Rec( const boost::shared_ptr<const Octree::OctreeNode>&octree,
												  std::vector<RobotRef > & robots_found,
												  double sq_radius,
												  const Vector3d & pos );

	/**
	 * Does the real Work on finding robots by radius.
	 *
	 * \param octree The octree to work on
	 * \param robots_found The set of already found robots. New found robots will be added to
	 * 		 this set.
	 * \param  sq_radius The square of the view radius
	 * \param pos The position of the robot
	 *
	 *
	 */
	static void get_visible_markers_by_radius_Rec( const boost::shared_ptr<const OctreeNode>&octree,
												   std::vector<MarkerRef > & markers_found,
												   double sq_radius,
												   const Vector3d & pos);

	/**
	 * Does the real Work on finding robots by radius.
	 *
	 * \param octree The octree to work on
	 * \param obstacles_found The set of already found obstacles. New found obstacles
	 * 		 will be added to this set.
	 * \param radius The  view radius
	 * \param pos The position of the robot
	 *
	 *
	 */
	static void get_visible_obstacles_by_radius_Rec( const boost::shared_ptr<const Octree::OctreeNode>&octree,
													 std::vector<ObstacleRef > & obstacles_found,
													 double radius,
													 const Vector3d & pos );



	/**
	 * Helper function to get rid of roots.
	 */
	static bool compare_to_squared_radius(const boost::shared_ptr<const Octree::OctreeNode>& octree,
											const Vector3d& pos,
											double sq_radius);

	/**
	 * Helper function to get rid of roots.
	 */
	static double calculate_squared_dist(const boost::shared_ptr<const Octree::OctreeNode>& octree,
										const Vector3d& pos);


};


#endif /* OCTREE_UTILITIES_H_ */
