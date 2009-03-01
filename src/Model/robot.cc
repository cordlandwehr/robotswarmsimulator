#include "robot.h"
#include "../Views/view.h"

Robot::~Robot() {

}

void Robot::set_view(const boost::weak_ptr<View>& view) {
	view_ = view;
}
