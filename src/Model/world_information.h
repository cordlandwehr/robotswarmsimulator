/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * \class	WorldInformation
 * \author	Martina Hüllmann
 * \brief	Each WorldInformation instance corresponds
 * 			to the state of the simulated world at a
 * 			specific simulation step.
 */
#ifndef WORLD_INFORMATION_H_
#define WORLD_INFORMATION_H_

#include <vector>
#include <map>
#include <boost/smart_ptr.hpp>


class Identifier;
class MarkerIdentifier;
class RobotIdentifier;
class EdgeIdentifier;
class MessageIdentifier;
class WorldObject;
class RobotData;
class Obstacle;
class Edge;
class Message;


/**
 * This class encapsulates information (as e.g. robot position, obstacles, ...) about the simulated world.
 *
 * Note that this class provides 'const' as well as 'non-const' access most of its data members. In fact, one can think
 * of it more as a struct than a class; the only possibly complex logic may be situated inside the more complex
 * acccessor methods like 'get_according_robot_data'.
 * The 'non-const' access is needed in the EventHandler to handle Request for a given robot. After the WorldInformation
 * leaves the EventHandler only constant references/pointers to it will be accessible.
 */
class WorldInformation {

public:
	WorldInformation();
	~WorldInformation();
	WorldInformation(const WorldInformation& rhs);

	/**
	 * Returns a constant reference to the set of the markers.
	 * \return Constant reference to the set of the markers.
	 */
	const std::vector<boost::shared_ptr<WorldObject> >& markers() const;

	/**
	 * Returns a (non-constant) reference to the set of markers.
	 * \return reference to the set of markers.
	 */
	std::vector<boost::shared_ptr<WorldObject> >& markers();

	/**
	 * Adds a new marker to the end of the current marker vector.
	 * \param Shared pointer to the new marker.
	 */
	void add_marker(boost::shared_ptr<WorldObject> new_marker);

	/**
	 * Sets the vector of markers in the world.
	 * \param Vector of markers to add to the world.
	 */
	void set_marker_data(std::vector<boost::shared_ptr<WorldObject> > new_markers);

	/**
	 * Returns a constant reference to the set of the obstacles.
	 * \return Constant reference to the set of the obstacles.
	 */
	const std::vector<boost::shared_ptr<Obstacle> >& obstacles() const;

	/**
	 * Returns a (non-constant) reference to the set of obstacles.
	 * \return reference to the set of obstacles.
	 */
	std::vector<boost::shared_ptr<Obstacle> >& obstacles();

	/**
	 * Adds a new obstacle to the end of the current obstacle vector.
	 * \param Shared pointer to the new obstacle.
	 */
	void add_obstacle(boost::shared_ptr<Obstacle> new_obstacle);

	/**
	 * Sets the vector of obstacles in the world.
	 * \param Vector of obstacles to add to the world.
	 */
	void set_obstacle_data(std::vector<boost::shared_ptr<Obstacle> > new_obstacles);

	/**
	 * Returns a constant reference to the set of the robot data.
	 * \return Constant reference to the set of the robots data.
	 */
	const std::map<std::size_t, boost::shared_ptr<RobotData> >& robot_data() const;
	
	/**
	 * Returns (in the given parameter vec) the set of the robot data as a vector.
	 * \param vec The vector to be used (should be empty)
	 */
	void robot_data_to_vector(std::vector<boost::shared_ptr<RobotData> >& vec) const;

	/**
	 * Returns a (non-constant) reference to the set of robot data.
	 * \return reference to the set of robot data.
	 */
	std::map<std::size_t, boost::shared_ptr<RobotData> >& robot_data();

	/**
	 * Adds a new robot data at its ID position.
	 * \param Shared pointer to the new robot data.
	 */
	void add_robot_data(boost::shared_ptr<RobotData> new_robot_data);

	/**
	 * Sets the set of robot data in the world.
	 * \param Vector of robot datas to add to the world.
	 */
	void set_robot_data(std::map<std::size_t, boost::shared_ptr<RobotData> > new_robot_data);
	
	/**
	 * Removes a robot data from the current set of robots. 
	 * All edges in connection with this robot will be removed, too.
	 * Also, all messages in this node's message queue will be removed.
	 * \param Shared pointer to edge
	 */
	void remove_robot_data(boost::shared_ptr<RobotIdentifier> robot_identifier);

	/**
	 * Returns a constant reference to the set of the edges.
	 * \return Constant reference to the set of the edges.
	 */
	const std::map<std::size_t, boost::shared_ptr<Edge> >& edges() const;

	/**
	 * Returns a (non-constant) reference to the set of edges.
	 * \return reference to the set of edges.
	 */
	std::map<std::size_t, boost::shared_ptr<Edge> >& edges();

	/**
	 * Adds a new edge to the end of the current edge map.
	 * \param Shared pointer to the new edge.
	 */
	void add_edge(boost::shared_ptr<Edge> new_edge);

	/**
	 * Sets the map of edges in the world.
	 * \param Map of edges to add to the world.
	 */
	void set_edge_data(std::map<std::size_t, boost::shared_ptr<Edge> > new_edges);

	/**
	 * Removes edge from edge map and according robots
	 * \param Shared pointer to edge
	 */
	void remove_edge(boost::shared_ptr<Edge> edge);

	/**
	 * Returns a constant reference to the set of the messages.
	 * \return Constant reference to the set of the messages.
	 */
	const std::map<std::size_t, boost::shared_ptr<Message> >& messages() const;

	/**
	 * Returns a (non-constant) reference to the set of messages.
	 * \return reference to the set of messages.
	 */
	std::map<std::size_t, boost::shared_ptr<Message> >& messages();

	/**
	 * Adds a new message to the end of the current message map.
	 * \param Shared pointer to the new message.
	 */
	void add_message(boost::shared_ptr<Message> new_message);

	/**
	 * Sets the map of messages in the world.
	 * \param Map of messages to add to the world.
	 */
	void set_message_data(std::map<std::size_t, boost::shared_ptr<Message> > new_messages);

	/**
	 * Removes message from message map and queue of according robot.
	 * \param Shared pointer to message
	 */
	void remove_message(boost::shared_ptr<Message> message);

	/**
	 * Returns the time (measured in steps) when this world info object was created.
	 * \return Time (measured in steps) when this world info object was created.
	 */
	int time() const;

	/**
	 * Setter for the time variable
	 * \param the new time
	 */
	void set_time(int time) {time_ = time;}

	/**
	 * \brief Checks whether the robot exists.
	 *
	 * Checks whether the robot identified by the RobotIdentifier id exists and return true if it does.
	 *
	 * \return true if robot exists, false otherwise
	 */
	bool robot_exists(boost::shared_ptr<RobotIdentifier> id) const;

	/**
	 * \brief Checks whether the edge exists.
	 *
	 * Checks whether the edge identified by the EdgeIdentifier id exists and return true if it does.
	 *
	 * \return true if edge exists, false otherwise
	 */
	bool edge_exists(boost::shared_ptr<EdgeIdentifier> id) const;


	/**
	 * \brief Checks whether the message exists.
	 *
	 * Checks whether the message identified by the MessageIdentifier id exists and return true if it does.
	 *
	 * \return true if message exists, false otherwise
	 */
	bool message_exists(boost::shared_ptr<MessageIdentifier> id) const;

	/**
	 * \brief Returns a constant reference to marker object with given id.
	 *
	 * This method assumes, that the marker object with the given id is saved at position id->id() in the markers
	 * vector.
	 *
	 * \return Constant reference to corresponding marker object.
	 */
	const WorldObject& get_according_marker(const MarkerIdentifier& id) const;

	/**
	 * \brief Returns a constant reference to marker object with given id.
	 *
	 * This method assumes, that the marker object with the given id is saved at position id->id() in the markers
	 * vector.
	 *
	 * \return Mutable reference to corresponding marker object.
	 */
	WorldObject& get_according_marker(const MarkerIdentifier& id);

	/**
	 * Return constant reference to according robotData of given robot ID
	 *
	 * This method assumes, that the according reference to the
	 * robotData of a robot with ID i is saved at position i in
	 * the robot_datas-vector.
	 *
	 * \param reference to identifier of robot whose robotData's reference shall be returned.
	 * \return Constant reference to according robotData of given robot ID.
	 */
	const RobotData& get_according_robot_data(boost::shared_ptr<RobotIdentifier> id) const;

	/**
	* Return (non-constant) reference to according robotData of given robot ID.
	*
	* This method assumes, that the according reference to the
	* robotData of a robot with ID i is saved at position i in
	* the robot_datas-vector.
	*
	* \param reference to identifier of robot whose robotData's reference shall be returned.
	* \return Mutable reference to according robotData of given robot ID.
	*/
	RobotData& get_according_robot_data(boost::shared_ptr<RobotIdentifier> id);
	
	
	/**
	 * Return constant pointer to according robotData of given robot ID
	 *
	 * This method assumes, that the according reference to the
	 * robotData of a robot with ID i is saved at position i in
	 * the robot_datas-vector.
	 *
	 * \param reference to identifier of robot whose robotData's reference shall be returned.
	 * \return Constant reference to according robotData of given robot ID.
	 */
	boost::shared_ptr<const RobotData> get_according_robot_data_ptr(boost::shared_ptr<RobotIdentifier> id) const;
	
	/**
	 * Return (non-constant) pointer to according robotData of given robot ID.
	 *
	 * This method assumes, that the according reference to the
	 * robotData of a robot with ID i is saved at position i in
	 * the robot_datas-vector.
	 *
	 * \param reference to identifier of robot whose robotData's reference shall be returned.
	 * \return Mutable reference to according robotData of given robot ID.
	 */
	boost::shared_ptr<RobotData> get_according_robot_data_ptr(boost::shared_ptr<RobotIdentifier> id);

	/**
	 * \brief Returns a constant reference to edge object with given id.
	 *
	 * \return Constant reference to corresponding edge object.
	 */
	boost::shared_ptr<const Edge> get_according_edge(boost::shared_ptr<EdgeIdentifier> id) const;

	/**
	 * Return (non-constant) reference to according edge of given edge ID.
	 *
	 * \param reference to identifier of edge which shall be returned.
	 * \return Mutable reference to according edge of given edge ID.
	 */
	boost::shared_ptr<Edge> get_according_edge(boost::shared_ptr<EdgeIdentifier> id);

	/**
	 * \brief Returns a constant reference to message object with given id.
	 *
	 * \return Constant reference to corresponding message object.
	 */
	boost::shared_ptr<const Message> get_according_message(boost::shared_ptr<MessageIdentifier> id) const;

	/**
	 * Return (non-constant) reference to according message of given message ID.
	 *
	 * \param reference to identifier of message which shall be returned.
	 * \return Mutable reference to according message of given message ID.
	 */
	boost::shared_ptr<Message> get_according_message(boost::shared_ptr<MessageIdentifier> id);

private:
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
	std::map< std::size_t, boost::shared_ptr<RobotData> > robot_data_;

	/**
	 * Set of edges in the world
	 */
	std::map<std::size_t, boost::shared_ptr<Edge> > edges_;

	/**
	 * Set of messages in the world
	 */
	std::map<std::size_t, boost::shared_ptr<Message> > messages_;

	/**
	 * Time (measured in steps) of creation of this world information
	 */
	int time_;

};

#endif /* WORLD_INFORMATION_H_ */
