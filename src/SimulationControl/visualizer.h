/*
 * visualizer.h
 *
 *  Created on: 19.01.2009
 *      Author: Daniel
 */

#ifndef VISUALIZER_H_
#define VISUALIZER_H_

class TimePoint;

/**
 * \brief Interface for visualization classes.
 */
class Visualizer {
public:
	virtual ~Visualizer() {}

	/**
	 * \brief Visualization classes have to overwrite this method to render the given world information.
	 *
	 * The given world information may be extrapolated using the given extrapolation time.
	 */
	virtual void draw(double extrapolation_time, const boost::shared_ptr<TimePoint> & time_point) = 0;

	/**
	 * \brief To be used for initialization.
	 */
	virtual void init() = 0;
	virtual void resize(int x, int y) = 0;
	virtual void mouse_func(int button, int state, int x, int y) = 0;
	virtual void keyboard_func(unsigned char key, int x, int y) = 0;
};

#endif /* VISUALIZER_H_ */
