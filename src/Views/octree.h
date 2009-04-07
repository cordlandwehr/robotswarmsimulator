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

#include "../Utilities/vector_arithmetics.h"

// Some forward declarations
class Obstacle;
class RobotData;
class WorldObject;
class OctreeUtilities;


/*!
 * \class	Octree
 * \author	Kamil Swierkot
 * \brief	Implements an Octree for WorldObjects.
 *
 * This class represents an octree. It subdivides recursive the space
 * around robots, markers, and obstacles.
 *
 * To create a new octree call the create_tree method with the objects to store
 * in the tree. The objects will be subdivided into several OctreeNodes corresponding
 * to their position in space. The recursive structure is represented by OctreeNodes. One
 * can use the tree() method to get the OctreeNodes. If a Node is subdivided, it has eight children.
 * The eight children of each node can be retrieved by the child() Method. For the possible
 * values see the enum OctreeNodes.
 *
 * The max depth of an octree can be specified by the max_level attribute. In order
 * to respect the view radius of the robots, one can specify the attribute min_width.
 * An OctreeNode will not be subdivided any more when the width of it would be smaller
 * than the min_width value.
 *
 *
 */
class Octree {

public:
	/*!
	 * \enum OctreeNodes
	 * \brief Gives Names to Child Nodes of the octree instead of numeric values
	 */
	enum OctreeNodes {

		TOP_LEFT_FRONT,
		TOP_LEFT_BACK,
		TOP_RIGHT_BACK,
		TOP_RIGHT_FRONT,
		BOTTOM_LEFT_FRONT,
		BOTTOM_LEFT_BACK,
		BOTTOM_RIGHT_BACK,
		BOTTOM_RIGHT_FRONT

	};


	/*!
	 * \brief Set up the Octree Node
	 *
	 *
	 */
	Octree();

	explicit Octree(int max_levels);

	explicit Octree(double min_width);

	/*!
	 * \brief Set up the Octree Node
	 *
	 * \param max_levels Used to set the maximum depth of the Octree
	 * \param min_width Used to set the min_width of an octree node
	 */
	Octree(int max_levels, double min_width);






	/**
	 * \brief This is the starting point of the tree creation.
	 *
	 * \param markers the markers to be stored in this tree
	 * \param obstacles the obstacles to be stored in this tree
	 * \param robot_datas the robot datas to be stored in this tree
	 */
	void create_tree(const std::vector< boost::shared_ptr<WorldObject> > & markers,
					 const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
					 const std::vector< boost::shared_ptr<RobotData> > & robot_datas);

	
	/**
	 * \brief Use this method to add a robot to the octree <em>after</em> its creation.
	 * \param robot Robot to add to the octree.
	 */
	void add_robot(boost::shared_ptr<RobotData> robot) {
		root_->add_robot(robot);
	}
	
	
	/**
	 * \brief Use this method to remove a robot from the octree.
	 * \param robot Robot to remove from the octree.
	 */
	void remove_robot(boost::shared_ptr<const RobotData> robot) {
		root_->remove_robot(robot);
	}


	/**
	 * This class represents the recursive structure of the octree.
	 * Robot datas, obstacles and markers are stored in this nodes.
	 * The only way to create new nodes is to create a new tree.
	 */
	class OctreeNode {
		
	public:
		OctreeNode() : level_(0), max_levels_(0), min_width_(0), sub_divided_(false), width_(0), object_count_(0) {
			center_ = boost::numeric::ublas::zero_vector<double>(3);
		};
		
		/**
		 * \brief Calculates the distance to this node from a given point.
		 *
		 * Note that this is an pessimistic approximation, in that the value returned may be less than the actual
		 * distance. More exactly, the distance from the point to the smallest enclosing sphere of the node is returned.
		 *
		 * \param pos The position
		 * \return Returns the distance to this node from the given point
		 */
		double calculate_dist_to_node(const Vector3d &pos) const;
		

		/**
		 * Returns the index of the child node in which the point would belong to
		 *
		 * \param pos The position of the point
		 *
		 */
		int point_in_node(const Vector3d & pos) const;


		/**
		 * Returns whether an obstacle would fit into this node or not
		 *
		 * \param obstacle the obstacle
		 */
		bool does_obstacle_fit(boost::shared_ptr<Obstacle> & obstacle) const ;

		/**
		 * This method allows to add objects to the octree.
		 *
		 * \param markers The new marker objects
		 * \param obstacles The new obstacles
		 * \param robot_datas The new robot_datas
		 */
		void assign_objects_to_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
									const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
									const std::vector< boost::shared_ptr<RobotData> > & robot_datas);

		/**
		 * Returns the marker stored in this node.
		 */
		const std::vector< boost::shared_ptr<WorldObject> > & marker_information() const {
			return markers_;
		}

		/**
		 * Returns the obstacles stored in this node.
		 */
		const std::vector< boost::shared_ptr<Obstacle> > & obstacles() const {
			return obstacles_;
		}

		/**
		 * Returns the robot datas stored in this node.
		 */
		const std::vector< boost::shared_ptr<RobotData> > & robot_datas() const {
			return robot_datas_;
		}

		/**
		 * This sets the parent of this node
		 *
		 * \param parent A pointer to the parent of this node
		 */
		void set_parent(OctreeNode * parent){
			parent_ = parent;
		}

		/**
		 * Returns the parent of this node.
		 * This is node a boost::shared_ptr
		 * because the child nodes get destroyed before their parent.
		 */
		const OctreeNode & parent() const {
			return *parent_;
		}

		/**
		 * Returns the depth of this node in the octree
		 */
		int level() const {
			return level_;
		}

		/**
		 * This returns the center of this node
		 */
		Vector3d center() const {
			return center_;
		}



		/**
		 * This return the number of objects which were stored in this node
		 */
		int object_count()  const {
			return object_count_;
		}



		/**
		 * This returns the width of this node (since it's a cube the height and depth are the same)
		 */
		double width() const {
			return width_;
		}


		/**
		 * This return whether the node is subdivided or not.
		 * If a node is subdivided, then it has all eight children.
		 */
		bool sub_divided()  const {
			return sub_divided_;
		}


		/**
		 * Returns the maximal depth of this octree
		 */
		int max_levels() const{
			return max_levels_;
		}

		/**
		 * Returns the minimal width of a node in this octree.
		 */
		double min_width() const{
			return min_width_;
		}

		/**
		 * Returns a child node.
		 *
		 * \param child_id The number of the child. Values in between 0 and 7 are legal numbers
		 */
		const boost::shared_ptr<OctreeNode> & child(int child_id ) const {
			return octree_nodes_[child_id];
		}
	protected:

		/*! \brief This sets the initial width, height and depth for the whole scene
		 *  \param markers The marker object in an vector of shared_ptr
		 *  \param obstacles The obstacles in an vector of shared_ptr
		 *  \param robot_datas The robot_datas in an vector of shared_ptr
		 */
		void scene_dimensions(const std::vector< boost::shared_ptr<WorldObject> > & markers,
                              const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
                              const std::vector< boost::shared_ptr<RobotData> > & robot_datas);

		
		/*! \brief This Method calculates the new center point of a node.
		 *
		 * It computes the center for the given child ID using the parent's (i.e. this node's) center and width.
		 * \param node_id The child ID of the node
		 */
		Vector3d new_node_center(int node_id);
		
		
		/*! \brief Adds the given robot to this subtree.
		 *
		 * This method allows the insertion of robots after the creation of the octree by Octree::create_tree.
		 * \param robot The robot to add to this subtree.
		 */
		void add_robot(boost::shared_ptr<RobotData> robot);
		
		
		/*! \brief Removes the given robot from this subtree (if contained).
		 *
		 * This method searches this subtree for the given robot and removes it, if found.
		 * \param robot Removes the given robot from this subtree (if contained).
		 */
		void remove_robot(boost::shared_ptr<const RobotData> robot);
		
		
		/*! Method to recursively build up the octree.
		 *
		 * \param markers the markers to store in this subtree
		 * \param obstacles the obstacles to store in this subtree
		 * \param robot_datas the robot_datas to store in this subtree
		 */
		void setup_node(const std::vector< boost::shared_ptr<WorldObject> > & markers,
                        const std::vector< boost::shared_ptr<Obstacle> > & obstacles,
                        const std::vector< boost::shared_ptr<RobotData> > & robot_datas);
		


		/**
		 * \brief This creates a new child node (with the given child ID).
		 *
		 * \param node_ide Child ID of the child to create.
		 */
		void create_new_node(int node_id);


		/**
		 * Sets the actual depth of this node.
		 * \param level The depth of this node
		 */
		void set_level(int level){
			level_ = level;
		}

		/**
		 * Sets the max depth of the tree
		 * \param max_level The maximal depth of the tree.
		 */
		void set_max_levels(int max_level) { max_levels_ = max_level; }

		/**
		 * Sets the minimal width of a node.
		 * \param min_width The minimal width of a node.
		 */
		void set_min_width(double min_width) { min_width_ = min_width; }



	private:
		/**
		 * \var parent_
		 * this stores an pointer to its parent.
		 */
		OctreeNode * parent_;

		/**
		 * \var level_
		 * The depth of this node.
		 */
		int level_;

		/**
		 * \var max_levels_
		 * The maximal depth of this tree
		 */
		int max_levels_;
		/**
		 * \var min_width_
		 * The minimum size of an node
		 */
		double min_width_;

		/**
		 * \var sub_divided_
		 * This tells whether the node has been subdivided or not.
		 * If this has been set to true, there won't be any robots and markers saved in this node
		 */
		bool sub_divided_;

		/**
		 * \var width_
		 * This is the size of the cube for this current node
		 */
		double width_;

		/**
		 * \var object_count_
		 * This holds the count of the objects in this node
		 */
		int object_count_;


		/**
		 * \var center_
		 * This is the center of the node
		 */
		Vector3d center_;

		/**
		 * \var markers_
		 * Set of markers in this node
		 */
		std::vector< boost::shared_ptr<WorldObject> > markers_;

		/**
		 * \var obstacles_
		* Set of obstacles in this node
		*/
		std::vector< boost::shared_ptr<Obstacle> > obstacles_;

		/**
		 * \var robot_datas_
		* Set of robot datas of robots in this node
		*/
		std::vector< boost::shared_ptr<RobotData> > robot_datas_;


		/**
		 * \var octree_nodes_
		 * This are the links to the child nodes
		 */
		boost::shared_ptr<OctreeNode> octree_nodes_[8];

		friend class Octree;

	};


	/**
	 * Returns the root of the octree
	 */
	const boost::shared_ptr<const OctreeNode > root() const {
			return root_;
	}


private:
	/**
	 * \var root_
	 * The root of the octree
	 */
	boost::shared_ptr<OctreeNode> root_;

	int max_levels_;
	double min_width_;

	friend class OctreeUtilities;

};

typedef Octree::OctreeNode OctreeNode;

#endif /* OCTREE_H_ */
