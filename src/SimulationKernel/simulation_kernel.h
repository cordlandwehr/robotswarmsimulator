/**
 * \class	SimulationKernel
 * \author	Martina Hüllmann
 * \brief	The central module of the Swarm–Simulator. Manages the data
 * 			and the progress of the simulated world.
 *
 */

#ifndef SIMULATION_KERNEL_H_
#define SIMULATION_KERNEL_H_

#include <vector>
#include <boost/smart_ptr.hpp>

using namespace std;

class Robot;
class Identifier;
class WorldInformation;
class History;

class SimulationKernel {
public:
	SimulationKernel();
	~SimulationKernel();

	/**
	 * Returns a constant reference to the set of the robots.
	 * \return Constant reference to the set of the robots.
	 */
	const vector<boost::shared_ptr<Robot> >& robots() const;

	/**
	 * Returns a constant reference to History of WorldInformations.
	 * \return Constant reference to History of WorldInformations.
	 *
	 * TODO(martinah) return History& instead of boost::shared_ptr<History>&
	 */
	const boost::shared_ptr<History>& history() const;

private:
	/**
	* Set of robots in the world
	*/
	std::vector< boost::shared_ptr<Robot> > robots_;

	/**
	 * Reference to a History of the WorldInformations.
	 */
	boost::shared_ptr<History> history_;
};

#endif /* SIMULATION_KERNEL_H_ */
