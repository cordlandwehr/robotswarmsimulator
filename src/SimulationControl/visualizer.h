/*
 * visualizer.h
 *
 *  Created on: 19.01.2009
 *      Author: Daniel
 */

#ifndef VISUALIZER_H_
#define VISUALIZER_H_

class Visualizer {
public:
	virtual ~Visualizer() {}
	virtual void draw(double extrapolation_time, boost::shared_ptr<WorldInformation> world_information) = 0;
	virtual void init() = 0;
};

#endif /* VISUALIZER_H_ */
