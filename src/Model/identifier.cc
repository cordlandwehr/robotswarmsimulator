#include "identifier.h"

Identifier::Identifier(std::size_t id) : id_(id){


}

Identifier::~Identifier() {

}

std::size_t Identifier::id() const {
	return id_;
}
