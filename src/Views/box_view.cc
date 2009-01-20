/*
 * box_view.cc
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#include "box_view.h"
#include "../Model/box.h"

BoxView::BoxView() {


}

BoxView::~BoxView() {

}

double BoxView::get_box_depth(const Box& box) const {
	return box.depth();
}

double BoxView::get_box_width(const Box& box) const {
	return box.width();
}

double BoxView::get_box_height(const Box& box) const {
	return box.height();
}
