/**
 * \class	Identifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots.
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <cstddef>
#include <boost/shared_ptr.hpp>

class Identifier {
public:
	virtual ~Identifier();

	virtual boost::shared_ptr<Identifier> clone() const = 0;

protected:
	explicit Identifier(std::size_t id);
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
