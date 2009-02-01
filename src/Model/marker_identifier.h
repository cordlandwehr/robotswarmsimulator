/**
 * \class	MarkerIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of markers.
 */

#ifndef MARKER_IDENTIFIER_H_
#define MARKER_IDENTIFIER_H_

#include "identifier.h"

class MarkerIdentifier : public Identifier {

	friend class SimpleWorldFixture;

public:
	~MarkerIdentifier();

	virtual boost::shared_ptr<Identifier> clone() const {
		return boost::shared_ptr<Identifier>(new MarkerIdentifier(*this));
	}
protected:
	explicit MarkerIdentifier(size_t id);
};

#endif /* MARKER_IDENTIFIER_H_ */
