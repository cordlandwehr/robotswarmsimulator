#include "sphere.h"
#include "math.h"


Sphere::Sphere(boost::shared_ptr<Identifier> id,
        boost::shared_ptr<Vector3d> position, double radius)
        : Obstacle(id, position), radius_(radius)
{


}

Sphere::Sphere(boost::shared_ptr<Identifier> id,
        boost::shared_ptr<Vector3d> position,
        boost::shared_ptr<MarkerInformation> marker_information, double radius)
        : Obstacle(id, position, marker_information), radius_(radius)
{


}

Sphere::~Sphere() {

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

boost::shared_ptr<WorldObject> Sphere::clone() const {
	return boost::shared_ptr<WorldObject>(new Sphere(*this));
}
