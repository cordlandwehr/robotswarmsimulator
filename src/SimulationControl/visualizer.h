/*
 * visualizer.h
 *
 *  Created on: 19.01.2009
 *      Author: Daniel
 */

#ifndef VISUALIZER_H_
#define VISUALIZER_H_

class WorldInformation;

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
	virtual void draw(double extrapolation_time, const boost::shared_ptr<WorldInformation> & world_information) = 0;

	/**
	 * \brief To be used for initialization.
	 */
	virtual void init() = 0;
};

#endif /* VISUALIZER_H_ */
