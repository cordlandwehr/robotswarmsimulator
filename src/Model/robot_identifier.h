/**
 * \class	RobotIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots.
 */

#ifndef ROBOT_IDENTIFIER_H_
#define ROBOT_IDENTIFIER_H_

#include "identifier.h"

class RobotIdentifier : public Identifier {
public:
	RobotIdentifier();
	~RobotIdentifier();
};

#endif /* ROBOT_IDENTIFIER_H_ */
