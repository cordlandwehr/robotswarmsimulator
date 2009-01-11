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
	~SphereIdentifier();
protected:
	SphereIdentifier(std::size_t id);
};

#endif /* SPHERE_IDENTIFIER_H_ */
