#include "sphere.h"
#include "math.h"

Sphere::Sphere(boost::shared_ptr<Identifier> id, double radius,
        boost::shared_ptr<Vector3d> position, boost::shared_ptr<MarkerInformation> marker_information)
        : Obstacle(id, position, marker_information), radius_(radius)
{
	// TODO Auto-generated constructor stub

}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}


double Sphere::radius() const {
	return radius_;
}

void Sphere::set_radius(double new_radius) {
	radius_ = new_radius;
}

bool Sphere::contains_point(boost::shared_ptr<Vector3d> point) {
	//Point is contained if
	//(point.x - position.x)^2 + (point.y - position.y)^2 + (point.z - position.z)^2 <= r^2
	/*
	 * TODO implement using type Vector3d
	if( pow(point[0]-position_[0], 2) +
	    pow(point[1]-position_[1], 2) +
	    pow(point[2]-position_[2], 2) <= pow(radius_, 2) )
		return true;
	else*/
		return false;
}
