#include "identifier.h"

Identifier::Identifier(std::size_t id) : id_(id){
	// TODO Auto-generated constructor stub

}

Identifier::~Identifier() {
	// TODO Auto-generated destructor stub
}

std::size_t Identifier::id() const {
	return id_;
}
