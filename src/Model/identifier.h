/**
 * \class	Identifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots and additionally distinguish
 * 			between owned and not owned ID's.
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

class Identifier {
public:
	Identifier();
	virtual ~Identifier();

	/**
	 * Return ID of identifier.
	 * \return ID of identifier.
	 */
	int id() const;

private:
	int id_;
};

#endif /* IDENTIFIER_H_ */
