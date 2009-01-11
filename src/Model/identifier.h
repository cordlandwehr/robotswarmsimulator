/**
 * \class	Identifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots.
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <cstddef>

class Identifier {
public:
	virtual ~Identifier() = 0;

protected:
	Identifier();
	/**
	 * Return ID of identifier.
	 * \return ID of identifier.
	 */
	std::size_t id() const;

private:
	std::size_t id_;

	friend class View;
	friend class WorldInformation; //TODO: needed?
};

#endif /* IDENTIFIER_H_ */
