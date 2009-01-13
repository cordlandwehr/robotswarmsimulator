#include "obstacle.h"

Obstacle::Obstacle(boost::shared_ptr<Identifier> id,
                   boost::shared_ptr<Vector3d> position)
        : WorldObject(id, position)
{


}

Obstacle::Obstacle(boost::shared_ptr<Identifier> id,
                   boost::shared_ptr<Vector3d> position,
                   boost::shared_ptr<MarkerInformation> marker_information)
        : WorldObject(id, position, marker_information)
{


}

Obstacle::~Obstacle() {
	// TODO Auto-generated destructor stub
}
