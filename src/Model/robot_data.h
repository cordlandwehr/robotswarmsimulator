/**
 * \class	RobotData
 * \author	Martina Hüllmann
 * \brief	Contains the properties a robot can have, but
 * 			which are not necessarily visible to the robot
 * 			and belong to the WorldInformation.
 */

#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <boost/tuple/tuple.hpp>
#include "../Utilities/vector_arithmetics.h"
#include "edge.h"
#include "undirected_edge.h"
#include "directed_edge.h"
#include "message.h"
#include <set>
#include <deque>


#include "world_object.h"

//TODO(martinah) set possible stati of a robot
//REMARK: Wanna have new Stati? You have to modify the robot_status_map_ in simulation_kernel.cc, too!
/**
 * \enum Status a robot can have.
 */
enum RobotStatus { SLEEPING, READY };

//TODO(martinah) set possible types of a robot
//REMARK: Wanna have new Robottypes? You have to modify the robot_type_map_ in simulation_kernel.cc, too!
/**
 * \enum Type a robot can have.
 */
enum RobotType { MASTER, SLAVE, kRobotTypeCount };

class Robot;
class MarkerInformation;
class View;
class RobotData : public WorldObject{
public:
	RobotData(boost::shared_ptr<Identifier> id,
	          boost::shared_ptr<Vector3d> position,
	          const Robot& robot);
	RobotData(boost::shared_ptr<Identifier> id,
			  boost::shared_ptr<Vector3d> position,
		      boost::shared_ptr<MarkerInformation> marker_information,
		      const Robot& robot);
	~RobotData();
	RobotData(const RobotData& rhs);

	/**
	 * Returns constant reference to acceleration vector of the robot.
	 * \return Constant reference to acceleration vector of the robot.
	 */
	const Vector3d & acceleration() const;

	/**
	 * Sets acceleration of the robot.
	 * \param Pointer to acceleration vector.
	 */
	void set_acceleration(boost::shared_ptr<Vector3d> new_acceleration);

	/**
	 * Returns constant reference to triple of pointers to vectors
	 * containing the coordinate system axis.
	 * \return Triple of vectors containing the coordinate system axis.
	 */
	const boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> >&
			coordinate_system_axis() const;

	/**
	 * Sets the coordinate system to the given triple of vectors.
	 * \param Triple of vectors for new axes.
	 */
	void set_coordinate_system_axis(boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
			boost::shared_ptr<Vector3d> > new_axes);

	/**
	 * Returns type of the robot.
	 * \return type of the robot.
	 */
	const RobotType type() const;



	/**
	 * Sets the type of the robot.
	 * \param new type
	 */
	void set_type(RobotType type) {type_ = type;}

	/**
	 * Returns constant reference to velocity vector of the robot.
	 * \return constant reference Velocity vector of the robot.
	 */
	const Vector3d & velocity() const;

	/**
	 * Sets velocity of the robot.
	 * \param Pointer to new velocity vector.
	 */
	void set_velocity(boost::shared_ptr<Vector3d> new_velocity);

	/**
	 * Returns position of the robot at the moment some timesteps in the future.
	 * \param double for additional timesteps
	 * \return Vector3d for coordinates
	 */
	boost::shared_ptr<Vector3d> extrapolated_position() const;

	/**
	 * Returns velocity of the robot at the moment some timesteps in the future.
	 * \param double for additional timesteps
	 * \return Vector3d for coordinates
	 */
	boost::shared_ptr<Vector3d> extrapolated_velocity(double timesteps) const;

	/**
	 * Returns status of the robot.
	 * \return Status of the robot.
	 */
	RobotStatus status() const;

	/**
	 * Sets status of the robot.
	 * \param new status
	 */
	void set_status(RobotStatus new_status);

	/**
	 * Returns reference to according robot-object.
	 * \return Reference to according robot-object.
	 */
	const Robot& robot() const;


	unsigned short int color(){
		return color_;
	}

	/**
	 * sets the view of this robot_data
	 */
	void set_view(boost::weak_ptr<View> view);

	/**
	 * accessor for the view of this robot data
	 */
	boost::weak_ptr<const View> view();

	void set_color( unsigned short int color){
		color_ = color;
	}
	
	/**
	 * Indicates wether the last request issued by this robot has been performed in (exactly) the way the robot
	 * intended.
	 */
	bool last_request_successful() const {
		return last_request_successful_;
	}
	
	/**
	 * To change the last_request_successful_ attribute, used to indicate wether the last request of this robot was
	 * successful.
	 */
	void set_last_request_successful(bool value) {
		last_request_successful_ = value;
	}

	virtual boost::shared_ptr<WorldObject> clone() const;

	/**
	 * Adds message so end of message queue
	 */
	void push_back_message(boost::shared_ptr<Message> m) {
		messages_.push_back(m);
	}

	/**
	 * Returns first message from queue
	 */
	boost::shared_ptr<Message> get_message() {
		return messages_.front();
	}

	/**
	 * Removes first message from queue
	 */
	void pop_front_message() {
		messages_.pop_front();
	}

	/**
	 * Adds edge to adjacency list
	 */
	void add_edge(boost::shared_ptr<Edge> e) {
		edges_.insert(e);
	}

	/**
	 * Removes edge from adjacency list
	 * TODO: Same shared_ptr needed -> check some other way?!?
	 */
	void remove_edge(boost::shared_ptr<Edge> e) {
		edges_.erase(e);
	}

	/**
	 * Returns constant set of edges
	 */
	const std::set<boost::shared_ptr<Edge> >& get_edges() {
		return edges_;
	}


private:
	/**
	 * Reference to according robot.
	 */
	//TODO (dwonisch): Do we really need this reference?
	const Robot& robot_;
	boost::shared_ptr<Vector3d> acceleration_;
	/**
	 * \var Triple with the three coordinate axes of the robot.
	 */
	boost::tuple<boost::shared_ptr<Vector3d>,
				 boost::shared_ptr<Vector3d>,
				 boost::shared_ptr<Vector3d> > coordinate_system_axis_;

	RobotType type_;
	boost::shared_ptr<Vector3d> velocity_;
	RobotStatus status_;
	bool last_request_successful_;

	boost::weak_ptr<View> view_;
	unsigned short int color_;

	std::set<boost::shared_ptr<Edge> > edges_;
	std::deque<boost::shared_ptr<Message> > messages_;
};

#endif /* ROBOT_DATA_H_ */
