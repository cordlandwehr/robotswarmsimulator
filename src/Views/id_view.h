/*
 * id_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef ID_VIEW_H_
#define ID_VIEW_H_

#include "own_id_view.h"

class IdView : public virtual OwnIdView {
public:
	IdView();
	virtual ~IdView();
protected:
	virtual std::size_t get_robot_id(const RobotData& robot) const;
	virtual std::size_t get_obstacle_id(const Obstacle& obstacle) const;
	virtual std::size_t get_marker_id(const WorldObject& marker) const;
};

#endif /* ID_VIEW_H_ */
