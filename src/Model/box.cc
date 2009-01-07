#include "box.h"

Box::Box() {
	// TODO Auto-generated constructor stub

}

Box::~Box() {
	// TODO Auto-generated destructor stub
}

double Box::length() const {
	return length_;
}

void Box::set_length(double new_length) {
	length_ = new_length;
}

double Box::depth() const {
	return depth_;
}

void Box::set_depth(double new_depth) {
	depth_ = new_depth;
}
