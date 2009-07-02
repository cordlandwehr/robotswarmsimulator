/*
 * color_change_request.h
 *
 *  Created on: 02.07.2009
 *      Author: kamil
 */

#ifndef COLOR_CHANGE_REQUEST_H_
#define COLOR_CHANGE_REQUEST_H_

#include "request.h"


class ColorChangeRequest : public Request {

public:
	ColorChangeRequest( Robot & robot, unsigned short int color ) : Request(robot), requested_color_(color){};

	unsigned short int requested_color() const { return requested_color_;}

private:
	unsigned short int requested_color_;

};


#endif /* COLOR_CHANGE_REQUEST_H_ */
