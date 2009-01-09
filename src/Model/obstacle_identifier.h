/**
 * \class	ObstacleIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of obstacles.
 */

#ifndef OBSTACLE_IDENTIFIER_H_
#define OBSTACLE_IDENTIFIER_H_

#include "identifier.h"

class ObstacleIdentifier : public Identifier {
public:
	ObstacleIdentifier();
	virtual ~ObstacleIdentifier();
};

#endif /* OBSTACLE_IDENTIFIER_H_ */
