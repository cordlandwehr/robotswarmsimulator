/**
 * \brief 	An instance of this class denotes an object
 * 			(robot, marker, obstacle) in the world.
 */
#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

#include <boost/smart_ptr.hpp>
#include <vector>

#include "marker_information.h"

using namespace std;

class WorldObject {
public:
	WorldObject();
	virtual ~WorldObject();
private:
	boost::shared_ptr<MarkerInformation> marker_information;
	std::vector<double> position;
};

#endif /* WORLD_OBJECT_H_ */
