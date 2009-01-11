#include "obstacle.h"

Obstacle::Obstacle(boost::shared_ptr<Identifier> id,
                   boost::shared_ptr<Vector3d> position,
                   boost::shared_ptr<MarkerInformation> marker_information)
        : WorldObject(id, position, marker_information)
{
	// TODO Auto-generated constructor stub

}

Obstacle::~Obstacle() {
	// TODO Auto-generated destructor stub
}
