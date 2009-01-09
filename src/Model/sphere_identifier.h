/**
 * \class	SphereIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of spheres.
 */

#ifndef SPHERE_IDENTIFIER_H_
#define SPHERE_IDENTIFIER_H_

#include "obstacle_identifier.h"

class SphereIdentifier : public ObstacleIdentifier {
public:
	SphereIdentifier();
	~SphereIdentifier();
};

#endif /* SPHERE_IDENTIFIER_H_ */
