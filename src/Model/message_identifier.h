/**
 * \class	MessageIdentifier
 * \author	Jonas
 * \brief	Denote ID's of messages.
 */

#ifndef MESSAGE_IDENTIFIER_H_
#define MESSAGE_IDENTIFIER_H_

#include "identifier.h"

class MessageIdentifier : public Identifier {
public:
	explicit MessageIdentifier();
	explicit MessageIdentifier(std::size_t id);

	MessageIdentifier(const MessageIdentifier& rhs) : Identifier(rhs) {};

	~MessageIdentifier();

	virtual boost::shared_ptr<Identifier> clone() const {
		return boost::shared_ptr<Identifier>(new MessageIdentifier(*this));
	}

private:
	static std::size_t message_id_counter_;
};



#endif /* MESSAGE_IDENTIFIER_H_ */
