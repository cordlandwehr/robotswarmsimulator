/**
 * \class	EdgeIdentifier
 * \author	Jonas
 * \brief	Denote ID's of edges.
 */

#ifndef EDGE_IDENTIFIER_H_
#define EDGE_IDENTIFIER_H_

#include "identifier.h"

class EdgeIdentifier : public Identifier {
public:
	explicit EdgeIdentifier(std::size_t id);
	~EdgeIdentifier();

	virtual boost::shared_ptr<Identifier> clone() const {
		return boost::shared_ptr<Identifier>(new EdgeIdentifier(*this));
	}


};



#endif /* EDGE_IDENTIFIER_H_ */
