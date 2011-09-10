#include "message_identifier.h"

MessageIdentifier::MessageIdentifier() : Identifier(message_id_counter_++) {
}

MessageIdentifier::MessageIdentifier(std::size_t id) : Identifier(id) {
}

MessageIdentifier::~MessageIdentifier() {
}

size_t MessageIdentifier::message_id_counter_ = 0;
