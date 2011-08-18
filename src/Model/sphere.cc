/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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

bool Sphere::contains_point(boost::shared_ptr<Vector3d> point) const {
	//Point is contained if
	//(point.x - position.x)^2 + (point.y - position.y)^2 + (point.z - position.z)^2 <= r^2
	if( pow((*point)(0)-(*position_)(0), 2) +
	    pow((*point)(1)-(*position_)(1), 2) +
	    pow((*point)(2)-(*position_)(2), 2) <= pow(radius_, 2) )
		return true;
	else
		return false;
}

boost::shared_ptr<WorldObject> Sphere::clone() const {
	return boost::shared_ptr<WorldObject>(new Sphere(*this));
}
