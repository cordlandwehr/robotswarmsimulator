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
	~RobotIdentifier();

	virtual boost::shared_ptr<Identifier> clone() const {
		return boost::shared_ptr<Identifier>(new RobotIdentifier(id()));
	}
protected:
	RobotIdentifier(std::size_t id);
};

#endif /* ROBOT_IDENTIFIER_H_ */