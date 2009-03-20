/*
 * robot_color.h
 *
 *  Created on: 20.02.2009
 *      Author: kamil
 */

#ifndef ROBOT_COLOR_H_
#define ROBOT_COLOR_H_

const unsigned int kRobotIdColorNum = 19;

/**
 * A list of colors to map color numbers to RGB triples
 */
const float kRobotIdColor [][3] = {
		{ 0.0f,1.0f,0.0f },
		{ 0.0f,0.0f,1.0f },

	{ 0.0f,1.0f,1.0f },
	{ 1.0f,0.0f,0.0f },
	{ 1.0f,0.0f,1.0f },
	{ 1.0f,1.0f,0.0f },
	{ 1.0f,1.0f,1.0f },
	{ 0.0f,0.0f,0.0f },
	{ 1.0f,0.5f,0.0f },
	{ 0.5f,0.0f,1.0f },

};


#endif /* ROBOT_COLOR_H_ */
