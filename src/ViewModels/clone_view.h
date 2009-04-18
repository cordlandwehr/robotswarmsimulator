/*
 * clone_view.h
 *
 *  Created on: 18.04.2009
 *      Author: Daniel
 */

#ifndef CLONE_VIEW_H_
#define CLONE_VIEW_H_

#include "../Views/spheric_view.h"
#include "../Views/id_view.h"
#include "../Views/time_view.h"
#include "../Views/position_view.h"
#include "../Views/velocity_view.h"

class CloneView: public virtual SphericView,
		public virtual IdView,
		public virtual TimeView,
		public virtual PositionView,
		public virtual VelocityView {
public:
	CloneView(double view_radius);
	virtual ~CloneView();
};

#endif /* CLONE_VIEW_H_ */
