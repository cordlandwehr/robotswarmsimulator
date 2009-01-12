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
	virtual ~ObstacleIdentifier();
	virtual boost::shared_ptr<Identifier> clone() const = 0;
protected:
	explicit ObstacleIdentifier(std::size_t id);
};

#endif /* OBSTACLE_IDENTIFIER_H_ */
