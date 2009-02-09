#include "box.h"

Box::Box(boost::shared_ptr<Identifier> id, boost::shared_ptr<Vector3d> position,
		 double depth, double width, double height)
        : Obstacle(id, position), depth_(depth), width_(width), height_(height)
{

}

Box::Box(boost::shared_ptr<Identifier> id, boost::shared_ptr<Vector3d> position, boost::shared_ptr<MarkerInformation> marker_information,
		 double depth, double width, double height)
        : Obstacle(id, position, marker_information), depth_(depth), width_(width), height_(height)
{

}


Box::~Box() {

}

double Box::height() const {
	return height_;
}

void Box::set_height(double new_height) {
	height_ = new_height;
}

double Box::depth() const {
	return depth_;
}

void Box::set_depth(double new_depth) {
	depth_ = new_depth;
}

double Box::width() const {
	return depth_;
}

void Box::set_width(double new_width) {
	depth_ = new_width;
}

boost::shared_ptr<WorldObject> Box::clone() const {
	return boost::shared_ptr<WorldObject>(new Box(*this));
}


bool Box::contains_point(boost::shared_ptr<Vector3d> point) const {
	//Point is contained if:
	//this.x - width/2 <= point.x <= this.x + width/2 AND
	//this.y - height/2 <= point.y <= this.y + height/2 AND
	//this.z - depth/2 <= point.z <= this.z + depth/2
	//remember: position is the center point of the box
	if( (*position_)(0) - width_/2. <= (*point)(0) && (*point)(0) <= (*position_)(0)+width_/2. &&
	    (*position_)(1) - height_/2. <= (*point)(1) && (*point)(1) <= (*position_)(1)+height_/2. &&
	    (*position_)(2) - depth_/2.<= (*point)(2) && (*point)(2) <= (*position_)(2)+depth_/2. )
		return true;
	else
		return false;
}
