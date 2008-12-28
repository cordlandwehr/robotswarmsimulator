/**
 * \brief	Each WorldInformation instance corresponds
 * 			to the state of the simulated world at a
 * 			specific simulation step.
 */
#ifndef WORLD_INFORMATION_H_
#define WORLD_INFORMATION_H_

#include <vector>
#include <boost/smart_ptr.hpp>

#include "world_object.h"
#include "robot.h"
#include "obstacle.h"

using namespace std;

class WorldInformation {
public:
	WorldInformation();
	virtual ~WorldInformation();

private:
	std::vector< boost::shared_ptr<WorldObject> > markers;
	std::vector< boost::shared_ptr<Obstacle> > obstacles;
	std::vector< boost::shared_ptr<Robot> > robots;
};

#endif /* WORLD_INFORMATION_H_ */
