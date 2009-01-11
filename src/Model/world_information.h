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
class RobotIdentifier;
class WorldObject;
class RobotData;
class Obstacle;


class WorldInformation {

public:
	WorldInformation();
	~WorldInformation();

	/**
	 * Returns a constant reference to the set of the markers.
	 * \return Constant reference to the set of the markers.
	 */
	const vector<boost::shared_ptr<WorldObject> >& markers() const;

	//TODO (martinah) maybe set_marker() instead of (or additionally?) this method
	/**
	 * Adds a new marker to the world.
	 * \param Shared pointer to the new marker.
	 */
	void add_marker(boost::shared_ptr<WorldObject> new_marker);

	/**
	 * Returns a constant reference to the set of the obstacles.
	 * \return Constant reference to the set of the obstacles.
	 */
	const vector<boost::shared_ptr<Obstacle> >& obstacles() const;

	//TODO (martinah) maybe set_obstacle() instead of (or additionally?) this method
	/**
	 * Adds a new obstacle to the world.
	 * \param Shared pointer to the new obstacle.
	 */
	void add_obstacle(boost::shared_ptr<Obstacle> new_obstacle);

	/**
	 * Returns a constant reference to the set of the robot data.
	 * \return Constant reference to the set of the robots data.
	 */
	const vector<boost::shared_ptr<RobotData> >& robot_data() const;

	//TODO (martinah) maybe set_robot_data() instead of (or additionally?) this method
	/**
	 * Adds a new robot data to the world.
	 * \param Shared pointer to the new robot data.
	 */
	void add_robot_data(boost::shared_ptr<RobotData> new_robot_data);

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
	 * Return constant reference to according robotData of given robot ID
	 *
	 * This method assumes, that the according reference to the
	 * robotData of a robot with ID i is saved at position i in
	 * the robot_datas-vector.
	 *
	 * TODO(craupach) match this parameter with the return value of robot.id(). Why does this take a raw pointer?
	 *
	 * \param reference to identifier of robot whose robotData's reference shall be returned.
	 * \return Constant reference to according robotData of given robot ID.
	 */
	const RobotData& get_according_robot_data(RobotIdentifier * id) const;

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
