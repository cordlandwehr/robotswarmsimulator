#include "edge_identifier.h"

EdgeIdentifier::EdgeIdentifier() : Identifier(edge_id_counter_++) {
}

EdgeIdentifier::~EdgeIdentifier() {
}

size_t EdgeIdentifier::edge_id_counter_ = 0;
