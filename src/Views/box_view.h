/*
 * box_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef BOX_VIEW_H_
#define BOX_VIEW_H_

#include "view.h"

class BoxView: public virtual View {
public:
	BoxView();
	virtual ~BoxView();
protected:
	virtual double get_box_depth(const Box& box) const;
	virtual double get_box_width(const Box& box) const;
	virtual double get_box_height(const Box& box) const;
};

#endif /* BOX_VIEW_H_ */
