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
#include <boost/smart_ptr.hpp>


using namespace std;

class Identifier;
class MarkerIdentifier;
class RobotIdentifier;
class WorldObject;
class RobotData;
class Obstacle;


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
	const vector<boost::shared_ptr<WorldObject> >& markers() const;

	/**
	 * Returns a (non-constant) reference to the set of markers.
	 * \return reference to the set of markers.
	 */
	vector<boost::shared_ptr<WorldObject> >& markers();

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
	const vector<boost::shared_ptr<Obstacle> >& obstacles() const;

	/**
	 * Returns a (non-constant) reference to the set of obstacles.
	 * \return reference to the set of obstacles.
	 */
	vector<boost::shared_ptr<Obstacle> >& obstacles();

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
	const vector<boost::shared_ptr<RobotData> >& robot_data() const;

	/**
	 * Returns a (non-constant) reference to the set of robot data.
	 * \return reference to the set of robot data.
	 */
	vector<boost::shared_ptr<RobotData> >& robot_data();

	/**
	 * Adds a new robot data to the end of the current RobotData vector.
	 * \param Shared pointer to the new robot data.
	 */
	void add_robot_data(boost::shared_ptr<RobotData> new_robot_data);

	/**
	 * Sets the vector of robot data in the world.
	 * \param Vector of robot datas to add to the world.
	 */
	void set_robot_data(std::vector<boost::shared_ptr<RobotData> > new_robot_data);

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
	std::vector< boost::shared_ptr<RobotData> > robot_data_;

	/**
	 * Time (measured in steps) of creation of this world information
	 */
	int time_;

};

#endif /* WORLD_INFORMATION_H_ */
