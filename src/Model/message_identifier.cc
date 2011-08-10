#include "message_identifier.h"

MessageIdentifier::MessageIdentifier() : Identifier(message_id_counter_++) {
}

MessageIdentifier::~MessageIdentifier() {
}

size_t MessageIdentifier::message_id_counter_ = 0;
