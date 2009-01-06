/**
 * \class WorldInformation
 * \brief	Each WorldInformation instance corresponds
 * 			to the state of the simulated world at a
 * 			specific simulation step.
 */
#ifndef WORLD_INFORMATION_H_
#define WORLD_INFORMATION_H_

#include <vector>
#include <boost/smart_ptr.hpp>

using namespace std;

class WorldObject;
class Robot;
class Obstacle;

class WorldInformation {

public:
	WorldInformation();
	virtual ~WorldInformation();

	/**
	 * Returns a constant reference to the set of the markers.
	 * \return Constant reference to the set of the markers.
	 */
	const vector<boost::shared_ptr<WorldObject> >& markers() const;

	//TODO (martinah) maybe set_marker() instead of (or additionally?) this method
	/**
	 * Adds a new marker to the world.
	 * \param a shared pointer to the new marker
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
	 * \param a shared pointer to the new obstacle
	 */
	void add_obstacle(boost::shared_ptr<Obstacle> new_obstacle);

	/**
	 * Returns a constant reference to the set of the robots.
	 * \return Constant reference to the set of the robots.
	 */
	const vector<boost::shared_ptr<Robot> >& robots() const;

	//TODO (martinah) maybe set_robot() instead of (or additionally?) this method
	/**
	 * Adds a new obstacle to the world.
	 * \param a shared pointer to the new robot
	 */
	void add_robot(boost::shared_ptr<Robot> new_robot);

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
	* Set of robots in the world
	*/
	std::vector< boost::shared_ptr<Robot> > robots_;
};

#endif /* WORLD_INFORMATION_H_ */
