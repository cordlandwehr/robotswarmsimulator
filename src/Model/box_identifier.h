/**
 * \class	BoxIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of boxes.
 */

#ifndef BOX_IDENTIFIER_H_
#define BOX_IDENTIFIER_H_

#include "obstacle_identifier.h"

class BoxIdentifier : public ObstacleIdentifier {

	friend class SimpleWorldFixture;

public:
	virtual ~BoxIdentifier();

	virtual boost::shared_ptr<Identifier> clone() const {
		return boost::shared_ptr<Identifier>(new BoxIdentifier(*this));
	}
protected:
	explicit BoxIdentifier(std::size_t id);
};

#endif /* BOX_IDENTIFIER_H_ */
