/*
 * chain_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef CHAIN_VIEW_H_
#define CHAIN_VIEW_H_

#include "position_view.h"
#include "neighbor_view.h"

/**
 * \class ChainView
 * \brief View model of the robot chain algorithm
 *
 * Assigning this class to a Robot corresponds to the robot chain view model, i.e. every Robot can see k neighbor Robots
 * position. Besides this no more information is visible.
 *
 * @see https://wiki.math.uni-paderborn.de/pg-schwarm/StartSeite/AK/Szenarien
 *
 */

class ChainView: public virtual PositionView, public virtual NeighborView {
public:
	ChainView(unsigned k);
	virtual ~ChainView();
};

#endif /* CHAIN_VIEW_H_ */
