/**
 * \class	BoxIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of boxes.
 */

#ifndef BOX_IDENTIFIER_H_
#define BOX_IDENTIFIER_H_

#include "obstacle_identifier.h"

class BoxIdentifier : public ObstacleIdentifier {
public:
	BoxIdentifier();
	virtual ~BoxIdentifier();
};

#endif /* BOX_IDENTIFIER_H_ */
