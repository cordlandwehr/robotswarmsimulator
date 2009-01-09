/**
 * \class	Identifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots and additionally distinguish
 * 			between owned and not owned ID's.
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <cstddef>

class Identifier {
public:
	Identifier();
	virtual ~Identifier();

	/**
	 * Return ID of identifier.
	 * \return ID of identifier.
	 */
	std::size_t id() const;

private:
	std::size_t id_;
};

#endif /* IDENTIFIER_H_ */
