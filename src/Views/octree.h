/*
 * \file octree.h
 *
 *  Created on: 08.01.2009
 *      Author: kamil
 */
#ifndef OCTREE_H_
#define OCTREE_H_

#include <vector>

#include <boost/smart_ptr.hpp>

#include "../Utilities/vector3d.h"

// Some forward declarations
class Obstacle;
class RobotData;
class WorldObject;



/*!
 * \class	Octree
 * \author	Kamil Swierkot
 * \brief	Implements an Octree for WorldObjects.
 *
 * TODO: Describe how to use this class....
 */
class Octree {

public:
	/*!
	 * \enum OctreeNodes
	 * \brief Gives Names to Child Nodes of the octree instead of numeric values
	 */
	enum OctreeNodes {

		TOP_LEFT_FRONT,			// 0
		TOP_LEFT_BACK,			// 1
		TOP_RIGHT_BACK,			// etc...
		TOP_RIGHT_FRONT,
		BOTTOM_LEFT_FRONT,
		BOTTOM_LEFT_BACK,
		BOTTOM_RIGHT_BACK,
		BOTTOM_RIGHT_FRONT

	};


	/*!
	 * \brief Set up the Octree Node
	 *
	 * max_levels_ is set to 20000 and min_width_ is set to 0.
	 */
	Octree();
	/*!
	 * \brief Set up the Octree Node
	 *
	 * \param max_levels Used to set the maximum depth of the Octree
	 * \param min_width Used to set the min_width of an octree node
	 */
	Octree(int max_levels, float min_width);
	~Octree();

	void destroy_octree();


	/*! \brief This sets the initial width, height and depth for the whole scene
	 *  \param markers The marker object in an vector of shared_ptr
	 *  \param obstacles The obstacles in an vector of shared_ptr
	 *  \param robot_datas The robot_datas in an vector of shared_ptr
	 */
	void scene_dimensions(std::vector< boost::shared_ptr<WorldObject> > & markers,
						  std::vector< boost::shared_ptr<Obstacle> > & obstacles,
						  std::vector< boost::shared_ptr<RobotData> > & robot_datas);



	/*! \brief This Method calculates the new center point of a node.
	 *
	 * This takes in the previous nodes center, width and which node ID that will be subdivided.
	 * The Result is the center of the new node.
	 * \param center The center of the parent node
	 * \param width  The width of the Octree Node box.
	 * \param node_id The ID of the node
	 */
	Vector3d new_node_center(Vector3d center, float width, int node_id);



	/*! This subdivides a node depending on the objects and node width
	 *
	 * \param markers the markers to store in this subtree
	 * \param obstacles the obstacles to store in this subtree
	 * \param robot_datas the robot_datas to store in this subtree
	 * \param center the center point of this node
	 * \param width the width of this node
	 *
	 */
	void create_node(std::vector< boost::shared_ptr<WorldObject> > & markers,
					 std::vector< boost::shared_ptr<Obstacle> > & obstacles,
					 std::vector< boost::shared_ptr<RobotData> > & robot_datas,
					 Vector3d center,
					 float width);



	/**
	 * \brief This creates a new node.
	 *
	 * \param markers The Markers to be stored in this subtree
	 * \param obstacles The obstacles to be stored in this subtree
	 * \param robot_datas The robot datas to be stored in this subtree
	 * \param center The center point of the new node
	 * \param width the width of the bounding box
	 */
	void create_new_node(std::vector< boost::shared_ptr<WorldObject> > & markers,
						 std::vector< boost::shared_ptr<Obstacle> > & obstacles,
						 std::vector< boost::shared_ptr<RobotData> > & robot_datas,
						 Vector3d center,
						 float width,
						 int node_id);



	float calculate_dist_to_node(const Vector3d &pos) const;

	bool does_obstacle_fit(boost::shared_ptr<Obstacle> & obstacle) const ;

	void assign_objects_to_node(std::vector< boost::shared_ptr<WorldObject> > & markers,
								std::vector< boost::shared_ptr<Obstacle> > & obstacles,
								std::vector< boost::shared_ptr<RobotData> > & robot_datas);


	void add_marker_to_node(boost::shared_ptr<WorldObject> marker);
	void add_obstacle_to_node(boost::shared_ptr<Obstacle> obstacle);
	void add_robot_data_to_node(boost::shared_ptr<RobotData> robot_data);


	std::vector< boost::shared_ptr<WorldObject> > & marker_information(){
		return markers_;
	}

	std::vector< boost::shared_ptr<Obstacle> > & obstacles() {
		return obstacles_;
	}


	std::vector< boost::shared_ptr<RobotData> > & robot_datas(){
		return robot_datas_;
	}

	void set_parent(boost::shared_ptr<Octree> & parent){
		parent_ = parent;
	}

	boost::shared_ptr<Octree> & parent(){
		return parent_;
	}

	int level(){
		return level_;
	}

	// This returns the center of this node

	Vector3d center() const {	 return center_;	}



	// This returns the triangle count stored in this node

	int object_count()  const {   return object_count_;	}



	// This returns the widht of this node (since it's a cube the height and depth are the same)

	float width() const {	 return width_;	}



		// This returns if this node is subdivided or not

	bool sub_divided()  const {   return sub_divided_;	}


	int max_levels(){return max_levels_;}
	float min_width(){return min_width_;}

	const boost::shared_ptr<Octree> & child(int child_id ) {
		return octree_nodes_[child_id];
	}


protected:
	void set_level(int level){
		level_ = level;
	}

	/**
	 * Sets the max depth of the tree
	 * \param max_level The maximal depth of the tree.
	 */
	void set_max_levels(int max_level){max_levels_ = max_level;};

	/**
	 * Sets the minimal width of a node.
	 * \param min_width The minimal width of a node.
	 */
	void set_min_width(float min_width){min_width_ = min_width;};

	/**
	 * \brief Helps to determin the type of an obstacle object.
	 *
	 * \param obstacle The obstacle to retrieve the real type of
	 * \return The type of the obstacle: 1 Box, 2 Sphere
	 */
	int determine_obstacle_type(boost::shared_ptr<Obstacle> & obstacle) const;

	/**
	 * \brief Returns the maximum size of an obstacle
	 * \param obstacle the obstacle
	 */
	float determine_obstacle_max_size(boost::shared_ptr<Obstacle> & obstacle) const;


private:

	// This initializes the data members
	void init_octree();


	/**
	 * this stores an pointer to its parent.
	 * TODO: Check if this has to be replaced by an raw pointer
	 */
	boost::shared_ptr<Octree> parent_;

	/**
	 * The depth of this node.
	 */
	int level_;
	/**
	 * The maximal depth of this tree
	 */
	int max_levels_;
	/**
	 * The minimum size of an node
	 */
	float min_width_;

	/**
	 * This tells whether the node has been subdivided or not.
	 * If this has been set to true, there won't be any robots and markers saved in this node
	 */
	bool sub_divided_;

	/**
	 * This is the size of the cube for this current node
	 */
	float width_;

	/**
	 * This holds the count of the objects in this node
	 */
	int object_count_;


	/**
	 * This is the center of the node
	 */
	Vector3d center_;

	/**
	 * Set of markers in the world
	 */
	std::vector< boost::shared_ptr<WorldObject> > markers_;

	/**
	* Set of obstacles in the world
	*/
	std::vector< boost::shared_ptr<Obstacle> > obstacles_;

	/**
	* Set of robot datas of robots in the world
	*/
	std::vector< boost::shared_ptr<RobotData> > robot_datas_;


	/**
	 * This are the links to the child nodes
	 */
	boost::shared_ptr<Octree> octree_nodes_[8];

};



#endif /* OCTREE_H_ */
